#include "time.h"
#include "led.h"

#include "rx.h"
#include "berry.h"
#include "error.h"
#include "pid.h"
#include "plant.h"
#include "esc.h"
#include "serial_tuning.h"
#include "imu9250.h"



///////////////////////////////////////////////////////////////////////////////
const unsigned long LOOP_MSEC= 4; //ms period to start  //might be able to go down without dbg?

bool Cal=false; //DONT CAHNGE THIS UNLESS YOU HAVE REMOVED THE PROPS FOR ESC CALIBRATION!!!!!  (see below)
rx Rx;
berry Berry(LOOP_MSEC);
error Error(LOOP_MSEC);
pid Pid(LOOP_MSEC);
plant Plant;
esc Esc;
serial_tuner Tuner;
MPU9250 IMU(0x68);
imu9250 Imu9250(LOOP_MSEC);

timer Timer(LOOP_MSEC);
led Led(LED_BUILTIN);
SBUS x8r(Serial2);




///////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);  // start serial for output  
  //Berry.setup();//detects and starts IMU
  x8r.begin();//starts SBUS objext
  Timer.setup();//sets time to 0
  Esc.setup();
  Tuner.setup(Pid.getRollConstants());
  Imu9250.setup();

}



///////////////////////////////////////////////////////////////////////////////
void loop() {



  //FLOW OF OPERATION IN FLIGHT CONTROLLER
  //berry+rx->error->pid->plant->motors
  //->led+otherstuff->timer
  

  


  Imu9250.run();
  //Imu9250.dbg();
  //Imu9250.dbgRaw();
  //Imu9250.dbgGyroRate();
  //Imu9250.dbgGyroPos();
  //Imu9250.dbgAcc();
  //Imu9250.dbgMag();
  
  if (Rx.getMode()==rx::idle){Imu9250.calcOffset();}


  
  //Berry.run();
  //Berry.dbg();


  
  
  Rx.run(Cal);
  //Rx.dbg();
  if (Rx.getMode()==rx::idle){Rx.setDesiredYaw(Imu9250.getYawOffset());}
  
  if (!Cal) Error.run( Imu9250.getMeasured(), Rx.getDesired() );//this error is measured - desired  
  else Error.run( Rx.getDesired(), Rx.getDesired() );//dbg only (error always 0)
  if (Rx.getTune()==rx::middle|| Rx.getMode()!=rx::fly) Error.clearI();
  //Error.dbg();
  //Error.dbgPitch();
  Error.dbgYaw();
  

  
  
  Pid.run(Error.getError4d());
  //Pid.dbg();

  Plant.run( Rx.getDesiredThrottle(), Pid.getResult() );
  //Plant.dbg();


  //based on RX MODE!
  if (Rx.getMode()==rx::fly){
    Esc.run(Plant.getThrottle());
    Led.setRate(5);    
  }
  else{
    Esc.idle();
    Led.setRate(100);
  }
  //Esc.dbg();



  Led.run(Timer.getEpoch());//blink rate

  Tuner.run();//recive pid tuning commands via serial
  if(Tuner.isAvailable()) Pid.setRollConstants(Tuner.getConstants());

  
  //print statments  epoch, e.roll.p, e.roll.i, e.roll.d, result.roll, kp, ki, kd
  if (Rx.getMode()!=rx::idle){
     /*
    Timer.csv();//epoch
    Error.csv();//e.roll.p, e.roll.i, e.roll.d 
    Pid.csv();//result.roll
    Pid.csvRollConstants();
    Serial.println("");
    // */
  }
  
  ////////////////////
  //Timed loop control
  //Serial.println("");  
  Timer.run(); //delay till timeout

  

  
}




