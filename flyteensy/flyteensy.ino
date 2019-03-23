
#include "rx.h"
#include "time.h"
#include "led.h"
//#include "berry.h"



///////////////////////////////////////////////////////////////////////////////
timer Timer(5);//5ms period to start
led Led(LED_BUILTIN);//100 loop period to start
SBUS x8r(Serial2);
rx Rx;
//berry Berry();


///////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);  // start serial for output  
  //Rx.setup();//does nothing xr8 below
  x8r.begin();
  Timer.setup();
  
}



///////////////////////////////////////////////////////////////////////////////
void loop() {
  //////////////////////
  //high time priority
  Rx.run();
  Rx.dbg();



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




