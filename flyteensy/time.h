#ifndef time_h
#define time_h

#include "Arduino.h"

class timer{
public:
  timer(unsigned long msPeriod){
    periodMs=msPeriod;
    startLoop=millis();  
    fastestLoop =1000000;
    slowestLoop=0;
  };
  
  void run(){
    epoch = epoch +1;      
    if(isExpired() ){
      Serial.print("TOO SLOW!!!!: loop time (ms)= ");Serial.println(millis() - startLoop);
    }  
    while(!isExpired() ) { delayMicroseconds(1); }    
    startLoop=millis();//retart the count for the next loop
  };
  
  void setup()  {
    startLoop=millis();  
    fastestLoop =1000000;
    slowestLoop=0;
  };
  
  void dbg(){
    unsigned long p=millis() - startLoop;    
    if( p> slowestLoop ){
      slowestLoop=p;
    }  
    if( p< fastestLoop ){
      fastestLoop=p;
    }    
    Serial.print("cur time ms:\t");
    Serial.print(p);
    Serial.print("\tslowest:\t");
    Serial.print(slowestLoop);
    Serial.print("\tfastest:\t");
    Serial.println(fastestLoop);
  };
  
  void setPeriodS(unsigned long s){
    periodMs=s*1000;
  };
  
  void setPeriodMs(unsigned long ms){
    periodMs=ms;
  };
  
  unsigned long getEpoch(){return epoch;};
  
  bool isExpired(){
    if(millis() - startLoop >= periodMs ) { return true;}
    else { return false; }    
  };
  
private:

  
  unsigned long periodMs;
  unsigned long startLoop;  
  unsigned long fastestLoop;
  unsigned long slowestLoop;
  unsigned long epoch;
};

#endif
