
#ifndef pid_error_h
#define pid_error_h



//////////////////////////////////////////////////////
//Types and Structs
struct pid_constants  //type
{
  float kp;
  float ki;
  float kd;
  float min;
  float max;
  
  void dbg(){
  Serial.print("kp: ");Serial.print(kp);Serial.print("\t\t");
  Serial.print("ki: ");Serial.print(ki);Serial.print("\t\t");
  Serial.print("kd: ");Serial.print(kd);Serial.print("\t\t");
  Serial.println("");
  };

  void csv(){
    Serial.print(kp); Serial.print(","); 
    Serial.print(ki); Serial.print(","); 
    Serial.print(kd); Serial.print(","); 
  }
   //...
};
typedef struct pid_constants PidConstants;




struct pid_error  //type
{
   float p;
   float i;
   float d;
   //...
   void dbg(){
    Serial.print("ep: ");Serial.print(p);Serial.print("\t\t");
    Serial.print("ei: ");Serial.print(i);Serial.print("\t\t");
    Serial.print("ed: ");Serial.print(d);Serial.print("\t\t");
    Serial.println("");
   };
   
   void csv(){
    Serial.print(p);Serial.print(",");
    Serial.print(i);Serial.print(",");
    Serial.print(d);Serial.print(",");
   };
};
typedef struct pid_error PidError;

struct error4d  //type
{
  pid_error roll;
  pid_error pitch;
  pid_error yaw;
  pid_error height;
};







#endif
