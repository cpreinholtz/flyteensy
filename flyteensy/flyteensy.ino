
#include "rx.h"
#include "time.h"
#include "led.h"
#include "berry.h"



///////////////////////////////////////////////////////////////////////////////
const unsigned long LOOP_MSEC= 5; //5ms period to start
timer Timer(LOOP_MSEC);
berry Berry(LOOP_MSEC);
led Led(LED_BUILTIN);
SBUS x8r(Serial2);
rx Rx;



///////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);  // start serial for output  
  //Rx.setup();//does nothing xr8 below
  Berry.setup();
  x8r.begin();
  Timer.setup();
  
}



///////////////////////////////////////////////////////////////////////////////
void loop() {
  //////////////////////
  //high time priority
  Rx.run();
  //Rx.dbg();

  Berry.run();
  Berry.dbg();



  //////////////////////
  //low priority 
  

  Led.run(Timer.getEpoch());

  /////////////////////
  //debug only  
  //Timer.dbg();

  ////////////////////
  //Timed loop control
  Timer.run();
  //delay(1);

}




