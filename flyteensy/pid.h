#ifndef pid_h
#define pid_h

#include "attitude.h"
#include "pid_error.h"

class pid{
public:
  pid(unsigned long msLoop){ 
    LOOP_PERIOD=((float)msLoop)/1000; 
    setupConstants();
  };
  
  
  
  void run(error4d e){
    pid_result.roll=get_pid_result(e.roll,kroll);
    pid_result.pitch=get_pid_result(e.pitch,kpitch);  
    pid_result.yaw=get_pid_result(e.yaw,kyaw); 
    //Add Height if GPS is integrated 
  };
  
  attitude getResult(){return pid_result;  };

  
  PidConstants getRollConstants(){return kroll;  };
  void setRollConstants(PidConstants con){
    kroll.kp=con.kp;
    kroll.ki=con.ki;
    kroll.kd=con.kd;
    kroll.max=con.max;
    kroll.min=con.min;
  };

  
  void dbg(){
    pid_result.dbg();    
  };
  
  void csv(){
    Serial.print(pid_result.roll); Serial.print(","); 
  };
  void csvRollConstants(){
    kroll.csv();
  };

private:
  PidConstants kroll,kpitch,kyaw, kheight;
  float LOOP_PERIOD;
  
  //this is not so much an attitude as it is just a container o floats for roll pitch yaw a throttle 
  attitude pid_result;


  //calculates kp*p+ki*i+kd*d and returns result after max and min is applied
  float get_pid_result(PidError err, PidConstants constant){
    float result=0.0;      
   
    result+=constant.kp*err.p;//P
    result+=constant.ki*err.i;//I
    result+=constant.kd*err.d; //D
    
    /*if (result>=constant.max){
      result=constant.max;
      Serial.println("PID MAX HIT!");
    }
    else if(result<=constant.min){
      result=constant.min;
      Serial.println("PID MIN HIT!");
    }*/
    return result;   
  };

  void setupConstants(){
      /////////////////////////////////////////////////////////////////
    //float kp =1.0;
    float kp =1.0;
    //0.5 feels too weak? no oscillation though 
    //kp =0.55;  very slow oscilation, probably the best so far
    //0.7 feels good, oscilation, present
    //0.6 feels nice with slight oscilation
    
    float ki =kp*0.50;//this is multiplied by LOOP_PERIOD
    //ki =0.0;
    //0.35 seems ok, maybe too high, I am getting oscilations
    //.05 feels too weak? too slow?
    //.15 feels good with kp = 1.0 and kd = 0.02.  think i need a bit more ki and less kd???
  
    
    float kd =kp*0.1;//this is divided by LOOP_PERIOD  //warning: error.d is already halfed
    
    //float kd =kp*0.3;//too high
    //float kd =kp*0.2;//pretty good
    //kd =0.0;
    //.05 too weak? 



    
    //better with 0.01
    //felt ok with 0.02
  //////////////////////////////////////////////////////////
    
    //Tune-able Parameters!!! 
    kroll.kp=kp;
    kroll.ki=ki;
    kroll.kd=kd;    
    //kroll.min= motor_max-motor_start *  -0.1 ;
    //kroll.max= motor_max-motor_start *   0.1 ;
  
    kpitch.kp=kp;
    kpitch.ki=ki;
    kpitch.kd=kd;
    //kpitch.min= motor_max-motor_start * -0.1 ;
    //kpitch.max= motor_max-motor_start *  0.1 ;
  
    //DO NOT CHANGE UNLESS YOU FIX THE DESIRED YAW, IT LOCKS TO 0 CURRENTLY
    kyaw.kp=0.00;
    kyaw.ki=0.0 * LOOP_PERIOD;
    kyaw.kd=0.0 / LOOP_PERIOD;
    //kyaw.min= motor_max-motor_start  *  -0.1 ;
    //kyaw.max= motor_max-motor_start *  0.1 ;
  
    kheight.kp=0.0;
    kheight.ki=0.0 * LOOP_PERIOD;
    kheight.kd=0.0 / LOOP_PERIOD;    
    //kheight.min= motor_max-motor_start *  -0.1 ;
    //kheight.max= motor_max-motor_start *   0.1 ;
  
  };

};

#endif
