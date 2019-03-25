#ifndef esc_h
#define esc_h
#include <PWMServo.h>
#include "throttle.h"


class esc{
public:
  esc(){};
  
  void run(Throttle t){
    usT=t;    
    
    degT=usToDeg(t);
    
    fl.write(degT.fl);
    fr.write(degT.fr);
    bl.write(degT.bl);
    br.write(degT.br);
  };
  
  void setup()  {
    fl.attach(3);
    fr.attach(4);
    bl.attach(5);
    br.attach(6);
    idle();
  };
  void dbg(){degT.dbg();};
  
  void idle(){
    fl.write(0);
    fr.write(0);
    bl.write(0);
    br.write(0);
  };
  
  void setEsc(float all){//be careful about deg vs uSec
    fl.write(all);
    fr.write(all);
    bl.write(all);
    br.write(all);
  };

private:


  float motor_min= 680;//uSeconds
  float motor_start= 821;//blades start turning
  float motor_max= 1400;
  
  PWMServo fl, fr, bl, br;

  Throttle usT,degT;
  

  int aMax=130;
  int aMin=50;
  Throttle usToDeg(Throttle t){
    t.fl=map(t.fl,motor_min,motor_max,aMin,aMax);
    t.fr=map(t.fr,motor_min,motor_max,aMin,aMax);
    t.bl=map(t.bl,motor_min,motor_max,aMin,aMax);
    t.br=map(t.br,motor_min,motor_max,aMin,aMax);  
    return t; 
  };

};

#endif
