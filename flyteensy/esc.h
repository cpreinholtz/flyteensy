#ifndef esc_h
#define esc_h
#include <PWMServo.h>
#include "throttle.h"


class esc{
public:
  esc(){};

  
  void run(Throttle t){
    setUs(t);//given usThrottle, convert to degThrottle then send to motors
  };
  
  void setup()  {
    //view the board from the top  (gnd is twords the board edge, sign is toword teensy)  (ONLY CONNECT 1 ESC PWR PIN AT A TIME!)
    //          (back)
    // ____________________________________
    //|
    //|
    //|   n/c n/c n/c n/c
    //|fl (3)
    //|fr (4)
    //|bl (5)
    //|br (6)
    //|
    //|
    //|___________________________________
    //          (front)
   
    fl.attach(3);
    fr.attach(4);
    bl.attach(5);
    br.attach(6);
    idle();
  };
  void dbg(){degThrottle.dbg();};
  
  void idle(){
    setAllEscDeg(degMin);    
  };



private:

  PWMServo fl, fr, bl, br;
  Throttle usThrottle, degThrottle;//holds "last" value

  
  //constants
  float motor_min= 680;//uSeconds
  float motor_start= 821;//blades start turning
  float motor_max= 1400;

  int degMax=130;//max throttle
  int degMin=50;//0 throttle



  //conversion functions
  Throttle usToDeg(Throttle t){
    //from ,to
    t.fl=map(t.fl,motor_min,motor_max,degMin,degMax);
    t.fr=map(t.fr,motor_min,motor_max,degMin,degMax);
    t.bl=map(t.bl,motor_min,motor_max,degMin,degMax);
    t.br=map(t.br,motor_min,motor_max,degMin,degMax);  
    return t; 
  };    
  
  Throttle degToUs(Throttle t){
    //from ,to
    t.fl=map(t.fl,degMin,degMax,motor_min,motor_max);
    t.fr=map(t.fr,degMin,degMax,motor_min,motor_max);
    t.bl=map(t.bl,degMin,degMax,motor_min,motor_max);
    t.br=map(t.br,degMin,degMax,motor_min,motor_max); 
    return t; 
  };


  //sets all to the same  (given in DEGREES!)
  void setAllEscDeg(float all){//be careful about deg vs uSec
    if (all<degMin)all=degMin;
    else if (all>degMax)all=degMax;
    degThrottle=all;
    usThrottle=degToUs(degThrottle);    
    fl.write(all);
    fr.write(all);
    bl.write(all);
    br.write(all);
  };


  //sets 4 different deg
  void setDeg(Throttle deg){
    usThrottle=degToUs(deg);       
    degThrottle=deg;
    
    fl.write(degThrottle.fl);
    fr.write(degThrottle.fr);
    bl.write(degThrottle.bl);
    br.write(degThrottle.br);    
  }

    //sets 4 different microsecon values
  void setUs(Throttle us){
    usThrottle=us;       
    degThrottle=usToDeg(us);
    
    fl.write(degThrottle.fl);
    fr.write(degThrottle.fr);
    bl.write(degThrottle.bl);
    br.write(degThrottle.br);    
  }
  

};

#endif
