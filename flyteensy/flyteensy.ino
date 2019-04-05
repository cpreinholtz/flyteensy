#include "time.h"
#include "led.h"

#include "rx.h"
#include "berry.h"
#include "error.h"
#include "pid.h"
#include "plant.h"
#include "esc.h"
#include "serial_tuning.h"



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

timer Timer(LOOP_MSEC);
led Led(LED_BUILTIN);
SBUS x8r(Serial2);




///////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);  // start serial for output  
  //Rx.setup();//does nothing xr8 below
  Berry.setup();//detects and starts IMU
  x8r.begin();//starts SBUS objext
  Timer.setup();//sets time to 0
  Esc.setup();
  Tuner.setup(Pid.getRollConstants());

}



///////////////////////////////////////////////////////////////////////////////
void loop() {



  //FLOW OF OPERATION IN FLIGHT CONTROLLER
  //berry+rx->error->pid->plant->motors
  //->led+otherstuff->timer
  

  


  Berry.run();
  //Berry.dbg();
  
  Rx.run(Cal);
  //Rx.dbg();
  
  
  if (!Cal) Error.run( Berry.getMeasured(), Rx.getDesired() );//this error is measured - desired  
  else Error.run( Rx.getDesired(), Rx.getDesired() );//dbg only (error always 0)
  if (Rx.getTune()==rx::middle) Error.clearI();
  //Error.dbg();

  
  
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
  if (Rx.getTune()!=rx::up){
    Timer.csv();//epoch
    Error.csv();//e.roll.p, e.roll.i, e.roll.d 
    Pid.csv();//result.roll
    Pid.csvRollConstants();
    Serial.println("");
  }
  
  ////////////////////
  //Timed loop control
  //Serial.println("");  
  Timer.run(); //delay till timeout

  

  
}




