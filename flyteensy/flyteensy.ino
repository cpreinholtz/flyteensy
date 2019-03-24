#include "time.h"
#include "led.h"

#include "rx.h"
#include "berry.h"
#include "error.h"
#include "pid.h"
#include "plant.h"
#include "esc.h"



///////////////////////////////////////////////////////////////////////////////
const unsigned long LOOP_MSEC= 5; //5ms period to start  //might be able to go down without dbg
rx Rx;
berry Berry(LOOP_MSEC);
error Error(LOOP_MSEC);
pid Pid(LOOP_MSEC);
plant Plant;
esc Esc;

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
  
}



///////////////////////////////////////////////////////////////////////////////
void loop() {
  //////////////////////
  //high time priority 


  Berry.run();
  //Berry.dbg();
  
  Rx.run();
  //Rx.dbg();
  
  Error.run( Berry.getMeasured(), Rx.getDesired() );//this error is measured - desired
  //Error.dbg();
  
  Pid.run(Error.getError4d());
  //Pid.dbg();

  Plant.run( Rx.getDesiredThrottle(), Pid.getResult() );
  //Plant.dbg();

  Esc.run(Plant.getThrottle());
  Esc.dbg();


  //////////////////////
  //low priority

  //if (!Timer.expired){
    Led.run(Timer.getEpoch());//blink rate
    //Rx.updateAux();
  //}
    



  ////////////////////
  //Timed loop control
  
  Timer.run(); //delay till timeout

  
}




