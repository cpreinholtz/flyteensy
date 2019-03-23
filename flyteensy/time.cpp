#include "Arduino.h"
#include "time.h"


timer::timer(unsigned long msPeriod){
  periodMs=msPeriod;
  startLoop=millis();  
  fastestLoop =1000000;
  slowestLoop=0;
}

void timer::run(){
  epoch = epoch +1; 
   
  if(millis() - startLoop > periodMs ){
    Serial.print("TOO SLOW!!!!: loop time (ms)= ");Serial.println(millis() - startLoop);
  }  
  while(millis() - startLoop < periodMs )    delayMicroseconds(1);
  startLoop=millis();
}

void timer::setup(){
  startLoop=millis();  
  fastestLoop =1000000;
  slowestLoop=0;
}


void timer::setPeriodS(unsigned long s){
  periodMs=s*1000;
}

void timer::setPeriodMs(unsigned long ms){
  periodMs=ms;
}










void timer::dbg(void){

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
}


