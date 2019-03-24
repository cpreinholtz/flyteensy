#ifndef error_h
#define error_h
#include "attitude.h"
#include "pid_error.h"
class error{
public:
  error(unsigned long msPeriod){
    LOOP_PERIOD=((float)msPeriod)/1000;    
    e.roll.p=0.0;
    e.roll.i=0.0;
    e.roll.d=0.0;        
    e.pitch.p=0.0;
    e.pitch.i=0.0;
    e.pitch.d=0.0;        
    e.yaw.p=0.0;
    e.yaw.i=0.0;
    e.yaw.d=0.0; 
    //Add height if GPS integration occours
  }
      
  void run(attitude measured, attitude desired){
    get_single_error(e.roll, normalise(measured.roll - desired.roll));
    get_single_error(e.pitch, normalise(measured.pitch - desired.pitch));  
    get_single_error(e.yaw, normalise(measured.yaw - desired.yaw));    
    //get_height_error();
  };

  void setup(){

  };

  error4d getError4d(){return e; };
  
  void dbg(){
    e.roll.dbg();
  };

private:
  const float i_error_max= 100.0;
  float LOOP_PERIOD;
  
  error4d e; //PidError eroll, epitch, eyaw, eheight;
  
  //gets errors for Attitudes

  ///////////////////////////////////////////////////////
  //ERROR AND PID
  void get_single_error(PidError & err, float temp ){

    err.d=(temp-err.p)/LOOP_PERIOD;
    
    err.p=temp;

    err.i+=temp*LOOP_PERIOD;
    if (err.i>=i_error_max)err.i=i_error_max;
    else if (err.i<=-i_error_max)err.i=-i_error_max;

  };
  
  float normalise(float err){//d, float m){
  
    //float err=m-d;
  
    if (err>180.0)err=err-360;
    else if (err<-180.0)err=err+360;
    return err;
  };

};

#endif
