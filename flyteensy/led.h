#ifndef led_h
#define led_h

#include "Arduino.h"



class led{
  
public:

  led(int pin){
    pinMode(pin, OUTPUT);
    rate=100;
  };
  
  void run(unsigned long epoch){
    static bool on=false;  
    if (epoch%rate==0) {
      if (on) {digitalWrite(LED_BUILTIN, HIGH); on=!on; }
      else {digitalWrite(LED_BUILTIN, LOW); on=!on; }
    }
  };
  void setRate(unsigned long r){rate=r;};

private:

  unsigned long rate;
  


};


#endif
