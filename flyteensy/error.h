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
    //deriv = a rolling avgerage  of this deriv (this p - last p)          and the last few derrivitives
    e.roll.d= edFir( normalise(measured.roll - desired.roll)-e.roll.p, edRollSamples, edRollIndex, e.roll.p);
    e.pitch.d= edFir( normalise(measured.pitch - desired.pitch)-e.pitch.p, edPitchSamples, edPitchIndex, e.pitch.p);
    e.yaw.d= edFir( normalise(measured.yaw - desired.yaw)-e.yaw.p, edYawSamples, edYawIndex, e.yaw.p);
    
    get_single_error(e.roll, normalise(measured.roll - desired.roll));
    get_single_error(e.pitch, normalise(measured.pitch - desired.pitch));  
    get_single_error(e.yaw, normalise(measured.yaw - desired.yaw));    
   

    
    //get_height_error();
  };

  void setup(){

  };

  error4d getError4d(){return e; };
  
  void clearI(){
    e.roll.i=0.0;
    e.pitch.i=0.0;
    e.yaw.i=0.0;
  };
  
  void dbg(){
    e.roll.dbg();
  };
  void dbgPitch(){
    e.pitch.dbg();
  };
  void dbgYaw(){
    e.yaw.dbg();
  };

  
  void csv(){
    e.roll.csv();
  };

private:
  //used to be 100 const float i_error_max= .0;
  const float i_error_max= 20;
  float LOOP_PERIOD;
  
  error4d e; //PidError eroll, epitch, eyaw, eheight;
  
  //gets errors for Attitudes

  ///////////////////////////////////////////////////////
  //ERROR AND PID  measured.roll - desired.roll...
  void get_single_error(PidError & err, float temp ){
    //temp = this measurment


    //float dRaw=(temp-err.p)/LOOP_PERIOD/10;
    
    //dRaw=dRaw/5;
    //if(dRaw<0) dRaw=-dRaw*dRaw;//square it and negate it!?!
    //else  dRaw=dRaw*dRaw;

    
    //float C=0.35; //this is the weight of the CURRENT d,  1.0-this is the IIR weight 
    //err.d=(C*dRaw)+((1-C)*err.d);    
    

    //preserve the archived calculation for reference
    //err.d=(temp-err.p)/LOOP_PERIOD;
    //err.d=(temp-err.p);
    
    err.p=temp;

    err.i+=temp*LOOP_PERIOD;
    if (err.i>=i_error_max)err.i=i_error_max;
    else if (err.i<=-i_error_max)err.i=-i_error_max;

  };


  static const int FirArrayLen=150;//make this bigger than target * 2
  int target=8;
  float weight = 1.0/float(target);
  
  float edRollSamples[FirArrayLen];
  int edRollIndex=0;
  
  float edPitchSamples[FirArrayLen];
  int edPitchIndex=0;
  
  float edYawSamples[FirArrayLen];
  int edYawIndex=0;
  
  
  float edFir(float thisD, float * samples, int & n, float thisP){
    float avg=0.0;


    
    samples[n]=thisD/LOOP_PERIOD/10;
    //normalization error at >180 difference  180/10~=10    
    if (samples[n]>=10/LOOP_PERIOD  ||samples[n]<=-10/LOOP_PERIOD ) samples[n]=0.0;//obvious normalization error
    
    //loop through the last few saples (target samples)
    for ( int i=n, count=0; count<target ;count++, i--){
      avg+=weight*samples[i];  
      if (i==0)i=FirArrayLen;      
    }
    //inc index for next time
    n++; if (n>=FirArrayLen) n=0;
    //return avg;

    
    //if (avg>0.0 && thisP>0.0) return 0.0;
    //if (avg<0.0 && thisP<0.0) return 0.0;

    return avg;
    //return edSquareFilter(avg);    
  }


  float edSquareFilter(float  thisD){
    //thisD=thisD/5;
    
    thisD=thisD/7.07;    
    if(thisD<0) {
      thisD=-thisD*thisD;//square it and negate it!?!
    }
    else  {
      thisD=thisD*thisD;
    }
    thisD=thisD*2;//THIS COMBINED WITH 7.07 PRESERVES THE 5
    
    
    return thisD;    
  }
  
  
  float normalise(float err){//d, float m){
  
    //float err=m-d;
  
    if (err>180.0) while (err>180.0) err=err-360;
    else if (err<-180.0) while (err<-180.0) err=err+360;
    return err;
  };

};

#endif
