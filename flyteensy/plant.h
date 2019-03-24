#ifndef plant_h
#define plant_h
#include "attitude.h"
#include "throttle.h"


class plant{
public:
  plant(){};
  
  void run(float baseline_throttle, attitude pid_result){

    raw_throttle=baseline_throttle;
    apply_baseline(baseline_throttle);
    apply_roll( pid_result);
    apply_pitch( pid_result);
    apply_yaw( pid_result);
    //apply_height( );
   
    limit_throttle();//dont get out of hand now children...
  };

  void setup();

  Throttle getThrottle(){return throttle;};
  
  void dbg(){
    Serial.print("throtle base:\t");Serial.print(raw_throttle);Serial.print("\t");
    throttle.dbg();
  };

private:
  Throttle throttle;  
  float raw_throttle;

  float motor_min= 680;//uSeconds
  float motor_start= 821;//blades start turning
  float motor_max= 1400;


  void apply_baseline(float desired_throttle){
    throttle.fl=desired_throttle;//base the throttle from desired
    throttle.fr=desired_throttle;
    throttle.bl=desired_throttle;
    throttle.br=desired_throttle;
  };


  
  //sides
  void apply_roll(attitude pid_result){
  //positive roll error means we are rolloed too far right(craft is moving right), increase right throttle
    throttle.fl-=pid_result.roll;
    throttle.bl-=pid_result.roll;
    
    throttle.fr+=pid_result.roll;
    throttle.br+=pid_result.roll;
  };
  
  //front/back 
  void apply_pitch(attitude pid_result){
  
  //positive pitch error means we are pitched too far forward(los is up and the craft is moving backward), increase back throttle
    throttle.fl-=pid_result.pitch;  
    throttle.fr-=pid_result.pitch;  
    
    throttle.bl+=pid_result.pitch;
    throttle.br+=pid_result.pitch;
  };
  
  
  //opposites
  void apply_yaw(attitude pid_result){
  
    //positive yaw error means we are to far cw, yaw the craft ccw by increasing throttle to cw motors(fl, and br)
    //cw motor spin
    throttle.fl+=pid_result.yaw;  
    throttle.br+=pid_result.yaw;
  
    //ccw motor spin
    throttle.fr-=pid_result.yaw;    
    throttle.bl-=pid_result.yaw;
  
  };
  
  //cap at max and min
  void limit_throttle(void){
    if (throttle.fl>=motor_max)throttle.fl=motor_max;
    else if (throttle.fl<=motor_min)throttle.fl=motor_min;
    
    if (throttle.fr>=motor_max)throttle.fr=motor_max;
    else if (throttle.fr<=motor_min)throttle.fr=motor_min;
    
    if (throttle.bl>=motor_max)throttle.bl=motor_max;
    else if (throttle.bl<=motor_min)throttle.bl=motor_min;
    
    if (throttle.br>=motor_max)throttle.br=motor_max;
    else if (throttle.br<=motor_min)throttle.br=motor_min;
  };



};

#endif

