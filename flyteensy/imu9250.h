
#ifndef imu9250_h
#define imu9250_h
//#include "IMU.h"
#include "Arduino.h"
#include "attitude.h"
#include "position.h"
#include "MPU9250.h"
#include "Wire.h"    // I2C library
//https://github.com/bolderflight/MPU9250  sauce


extern MPU9250 IMU;


class imu9250{
public:

  imu9250(unsigned long msLoop){ LOOP_PERIOD=((float)msLoop)/1000;};
  
  void run(){  
    IMU.readSensor();         
    fuse_sensors();
    apply_offsets(); 
  };

  void calcOffset(){
    float thisWeight=0.5;    
    offset.pitch=(cfilter.x*(thisWeight))+(offset.pitch*(1-thisWeight));
    offset.roll=(cfilter.y*(thisWeight))+(offset.roll*(1-thisWeight));
    offset.yaw=(cfilter.z*(thisWeight))+(offset.yaw*(1-thisWeight));
    
  }
  
  
  void setup(){
    Serial.println("Setting up 9250");
    // start communication with IMU 
    int status = IMU.begin();
    if (status < 0) {
      Serial.println("IMU initialization unsuccessful");
      Serial.println("Check IMU wiring or try cycling power");
      Serial.print("Status: ");
      Serial.println(status);
      while(1) {}
    }
    // setting the accelerometer full scale range to +/-8G 
    IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
    // setting the gyroscope full scale range to +/-500 deg/s
    IMU.setGyroRange(MPU9250::GYRO_RANGE_2000DPS);//250,500,1000,2000
    // setting DLPF bandwidth to 20 Hz
    IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
    // set SRD to 19 for a 50 Hz update rate (divider) 
    IMU.setSrd(4);

  };
  
  void dbgRaw(){
        // display the data
    Serial.print(IMU.getAccelX_mss(),6);
    Serial.print("\t");
    Serial.print(IMU.getAccelY_mss(),6);
    Serial.print("\t");
    Serial.print(IMU.getAccelZ_mss(),6);
    Serial.print("\t");
    Serial.print(IMU.getGyroX_rads(),6);
    Serial.print("\t");
    Serial.print(IMU.getGyroY_rads(),6);
    Serial.print("\t");
    Serial.print(IMU.getGyroZ_rads(),6);
    Serial.print("\t");
    Serial.print(IMU.getMagX_uT(),6);
    Serial.print("\t");
    Serial.print(IMU.getMagY_uT(),6);
    Serial.print("\t");
    Serial.print(IMU.getMagZ_uT(),6);
    Serial.print("\t");
    Serial.println(IMU.getTemperature_C(),6);

  };
  void dbg(){
    measured.dbg();
  };
  
  void dbgGyroRate(){
    gyro_rate.dbg();    
  };
  
  void dbgGyroPos(){
    gyro_pos.dbg();
  };
  
  void dbgAcc(){
    acc_pos.dbg();
  };
  
  void dbgMag(){
    Serial.print("Mag heading\t");
    Serial.println(heading,6);
  };

  float getYawOffset(){return offset.yaw;};

  attitude getMeasured(){ return measured;  };


  

private:
  

  float LOOP_PERIOD;  //seconds per loop
  bool CalibrateAngle=true;
  const bool UPSIDEDOWN=false;
    
  const float G = 0.95;         // complementary filter constant
  const float A = (1-G);        // complementary filter constant




  Fcart acc_pos;
  Fcart gyro_rate;
  Fcart gyro_pos={0.0,0.0,0.0};
  Fcart cfilter={0.0,0.0,0.0};
  attitude measured;
  attitude offset={0.0,0.0,0.0,0.0};
  float heading;//straight from magnotometer


  //////////////////////////////////////////////////
  //gets all sensor readings
  void fuse_sensors(){  
    acc_pos = get_acc_pos();
    gyro_pos=get_gyro_pos();
    get_gyro_rate();
    heading=get_mag();
    
    cf(cfilter.x, acc_pos.x, gyro_pos.x);//comlimentary filter
    cf(cfilter.y, acc_pos.y, gyro_pos.y);

    //heading will always be b/w 0 and 360 bc of trigg in the function
    //problem is gpos can get above 360 (last cf is fedback and added to gyro rotation)
    
    if ((gyro_pos.z >300.0) && (heading < 60.0) ) while (gyro_pos.z >300.0) gyro_pos.z-=360.0;
    else if ((gyro_pos.z <60.0)&& (heading >300.0) ) while (gyro_pos.z <60.0) gyro_pos.z+=360.0;//not sure why we "trust" the mag more but it will be corrected later anywho    
       

    
    //cf(cfilter.z, heading, gyro_pos.z);
    //cfilter.z=heading;//trust mag 100%...
    float trustMag=0.05;
    cfilter.z=heading*trustMag+(1-trustMag)*gyro_pos.z;//trust mag 15%...
    
    if (cfilter.z<0.0) while (cfilter.z<0.0)  cfilter.z+=360; //this protect us from getting - headings or headings over 360 (not that it should matter)
    else if(cfilter.z>360.0) while (cfilter.z>360.0) cfilter.z-=360; 

    

    

  };

    //////////////////////////////////////////////////
  //gets all sensor readings
  void apply_offsets(){  
    measured.pitch=cfilter.x-offset.pitch;
    measured.roll=cfilter.y-offset.roll;    
    //measured.yaw=cfilter.z-offset.yaw;
    measured.yaw=cfilter.z;
    measured.throttle=0.0;
  };
  

  
  
  
  ///////////////////////////////////////////////////////////
  //ACC:  convert m/s^2 to degrees by using simple 3d trig
  Fcart get_acc_pos(void){

    Fcart a_trans;
      
    a_trans.x = ( (float) (atan2(IMU.getAccelX_mss(),-IMU.getAccelZ_mss()))  ) *RAD_TO_DEG;
    a_trans.y = ( (float) (atan2(IMU.getAccelY_mss(),-IMU.getAccelZ_mss()))  ) *RAD_TO_DEG;
    return a_trans;

  };
  
  

  
  
  
  float get_single_gyro_pos(float last, float rate ){

    float delta =LOOP_PERIOD * rate;  
    float pos= ( delta  + last);     
    return pos;
  };

  Fcart get_gyro_rate(){
    gyro_rate.x=  IMU.getGyroY_degs();
    gyro_rate.y= -IMU.getGyroY_degs();
    gyro_rate.z=  IMU.getGyroZ_degs(); 
    return gyro_rate;
  };
  
  
  Fcart get_gyro_pos(void){
    Fcart g_pos;

    //According to https://github.com/bolderflight/MPU9250
    //Gyro +x will decrease acc Y (tip y side up)
    //Gyro +y will incerease acc X! (tip x side down)
    //Gyro +z will increase heading yaw right (increase mag heading)
    
    //use acc top view as guide:
    //    +x (acc)           (FOWRWARD!)
    //   __^___ (acc)    
    //  |      |
    //  |  Xz  | ---> +y (acc)
    //  |_____*|
    //  

    //On breakout:

    //+x= +pitch = tip nose down
    //+y= +roll = tip rightside down
    
    //    +x (acc)           
    //  |   __^___        O |
    //  |  |      |       O |
    //  |  |  Xz  |-> +y  O |
    //  |  |_____*|       O |
    //  |                 O |
    //  |                 O |
    
    
    g_pos.x=get_single_gyro_pos(cfilter.x,   IMU.getGyroY_degs());//according to contols theory, feedback the filtered measurment when getting the gyro position    
    g_pos.y=get_single_gyro_pos(cfilter.y,  -IMU.getGyroX_degs());
    g_pos.z=cfilter.z+  (IMU.getGyroZ_degs()*LOOP_PERIOD);

    
    return g_pos;

  };
  
  
  
  
  ///////////////////////////////////////////////////////////
  //MAG (x,y vector to heading)
  float get_mag (void){
    float head;
    head = atan2(IMU.getMagY_uT(),-IMU.getMagX_uT()) *RAD_TO_DEG; 
    if(head < 0) head += 360; //this should yeild a 0 to 360 
    return head;

  };
  















  //////////////////////////////////////////////////
  //COMP FILTER
  void cf(float &val, float a, float g){
    
    //A= 1-G;
    val= g*G + a*A;
   
  };

  
  
  
  ///////////////////////////////////////////////////////////
  //conver 2 8bit bytes into int with signed extension
  
  int bytes_to_int(byte ls, byte ms){
  
    if (ms&0x80){ //negative check
      int val;
      val =-1;
      val =val xor 0xFFFF;
      //int val=0xFFFF0000;//only works with 32 bit architechture & 16 bits of data!
      
      val=val | (int(ms)<<8) ;//should be all ones at this point, so anding is prob what we need
      val=val| ((int)ls) ;
      return val;
    }
    else{
      int val=int(ms);
      val=val << 8;
      val=val| ((int)ls) ;
      return val;
    }  
  };

 
};



#endif

