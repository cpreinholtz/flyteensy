
#ifndef berry_h
#define berry_h
#include "IMU.h"
#include "attitude.h"
#include "position.h"







class berry{
public:
  berry(unsigned long msLoop){ LOOP_PERIOD=((float)msLoop)/1000;};
  
  void run(){        
    poll_sensors();
        
    measured=sensor_to_craft(cfilter);
    if(CalibrateAngle)calc_offsets();
  };
  
  void setup(){
    Serial.println("Setting up BerryIMU");
    detectIMU();
    enableIMU(); 

    offset.yaw=0.0;
    offset.roll=0.0;
    offset.pitch=0.0;
  };
  
  void dbg(){
    measured.dbg();
  };

  attitude getMeasured(){ return measured;  };


  

private:

  attitude offset, measured;

  bool CalibrateAngle=true;
  bool UPSIDEDOWN=true;
  float LOOP_PERIOD;  //seconds per loop
  const float G = 0.99;         // complementary filter constant
  const float A = (1-G);        // complementary filter constant
  float G_GAIN=0.070;
  
  float q_trans[3][3]={
    {1,0,0},
    {0,1,0},
    {0,0,1}
  };
  
  float heading;//heading
  Fcart acc_pos;
  Fcart gyro_rate;
  Fcart gyro_pos={0.0,0.0,0.0};
  Fcart cfilter={0.0,0.0,0.0};
  Icart gyro_raw, acc_raw, mag_raw;

  //////////////////////////////////////////////////
  //gets all sensor readings
  void poll_sensors(){  
    
    acc_pos=get_acc_pos();
    gyro_rate=get_gyro_rate();
    heading=get_mag();
    gyro_pos=get_gyro_pos();
    
    cf(cfilter.x, acc_pos.x, gyro_pos.x);//comlimentary filter
    cf(cfilter.y, acc_pos.y, gyro_pos.y);


    //heading will always be b/w 0 and 360 bc of trigg in the function
    //problem is gpos can get above 360 (last cf is fedback and added to gyro rotation)
    if ((gyro_pos.z >300) && (heading < 60) ) gyro_pos.z-=360.0;
    else if ((gyro_pos.z <60)&& (heading >300) ) gyro_pos.z+=360.0;//not sure why we "trust" the mag more but it will be corrected later anywho
    
    cf(cfilter.z, heading,  gyro_pos.z); //gyro_pos.z);    
    
      if (cfilter.z<0.0) cfilter.z+=360; //this protect us from getting - headings or headings over 360 (not that it should matter)
      else if(cfilter.z>360.0) cfilter.z+=360; 
    
  };
  
  

  
  /////////////////////////////////////////////
  //ACC
  Icart get_acc_vector(){
    Icart a;
    byte buff[6];  
    readACC(buff);  
    if (UPSIDEDOWN){//rotate sensore 180 deg about the x axis, invert and z and x ( because of the atan2 inverting z flips both x and y but we have the same x and opposite y so invert those)
      a.x=- bytes_to_int(buff[0] , buff[1]) ;
      a.y=- bytes_to_int(buff[2] , buff[3]) ;
      a.z=- bytes_to_int(buff[4] , buff[5]) ;//craft top is -z dir
    }else{
      a.x=  bytes_to_int(buff[0] , buff[1]) ;//left is +x dir (left down is +roll)
      a.y=- bytes_to_int(buff[2] , buff[3]) ; //front is -y dir (front down is + pitch)
      a.z=  bytes_to_int(buff[4] , buff[5]) ; //craft top is +z dir
    } 
    /*DONT TOUCH (original vidor) this is the vidor setup
    a.x=  bytes_to_int(buff[0] , buff[1]) ;//right is -x dir
    a.y=- bytes_to_int(buff[2] , buff[3]) ; //front is -y dir
    a.z=  bytes_to_int(buff[4] , buff[5]) ;  */
    return a;
  };


  ///////////////////////////////////////////////////////////
  //GYRO
  Icart get_gyro_rate_vector(){
    Icart raw;
    byte buff[6]; 
    readGYR(buff);

    //upside up
            
    //     back 
    // --------------
    //|_ _ _  |___|   |left
    //|-|-|-|         |  
    // ---------------
    //  front
    // 


    //upside down (left and right remain the same) front/ up are swapped  (rotate about x axis, no change in x)
    
    //     back  
    // --------------
    //|  {}   \      |left
    //|  {}   /      | 
    // ---------------
    //  front
    // 
    //+xrotation 
    
    if (UPSIDEDOWN){//flip about the x axis, invert y and z
      raw.x = - bytes_to_int(buff[2] , buff[3] );       
      raw.y =   bytes_to_int(buff[0] , buff[1] );
      raw.z =   bytes_to_int(buff[4] , buff[5] );
    } else{
      raw.x = - bytes_to_int(buff[2] , buff[3] );       
      raw.y = - bytes_to_int(buff[0] , buff[1] );
      raw.z = - bytes_to_int(buff[4] , buff[5] );
    }
    /*DONT TOUCH (original vidor)
    raw.x = - bytes_to_int(buff[2] , buff[3] );   
    raw.y = - bytes_to_int(buff[0] , buff[1] );
    raw.z = - bytes_to_int(buff[4] , buff[5] );*/
    return raw;
  };
  
  ///////////////////////////////////////////////////////////
  //MAG
  Icart get_magnetic_vector(){
    Icart m;
    byte buff[6]; 
    readMAG(buff);
    if (UPSIDEDOWN){//rotate about x axis, negate y and z
      m.x =   bytes_to_int(buff[0] , buff[1] );   
      m.y = - bytes_to_int(buff[2] , buff[3] );
      m.z = - bytes_to_int(buff[4] , buff[5] );  
    }else {
      m.x =   bytes_to_int(buff[0] , buff[1] );   
      m.y =   bytes_to_int(buff[2] , buff[3] );
      m.z =   bytes_to_int(buff[4] , buff[5] ); 
    }           
    return m;
  };




 
  
  
  
  ///////////////////////////////////////////////////////////
  //ACC convert to degrees
  Fcart get_acc_pos(void){
    Icart a=get_acc_vector();
    Fcart a_trans;
      
    a_trans.x = ( (float) (atan2(a.x,a.z))  ) *RAD_TO_DEG;
    a_trans.y = ( (float) (atan2(a.y,a.z))  ) *RAD_TO_DEG;
    return a_trans;
  };
  
  
  ///////////////////////////////////////////////////////////
  //Gyro position
  
  Fcart get_gyro_rate(){
    Fcart g;
    gyro_raw=get_gyro_rate_vector();
    
    //Convert Gyro raw to degrees per second
  
    g.x = ((float)gyro_raw.x) * G_GAIN;   
    g.y = ((float)gyro_raw.y) * G_GAIN; 
    g.z = ((float)gyro_raw.z) * G_GAIN; 
    
  
    return g;
  };
  
  
  
  float get_single_gyro_pos(float last, float rate ){
    
    float delta =LOOP_PERIOD * rate;  
    float pos= ( delta  + last);
     
    return pos;
  };
  
  
  Fcart get_gyro_pos(void){
    Fcart g_pos;
    g_pos.x=get_single_gyro_pos(cfilter.x,gyro_rate.x);//according to contols theory, feedback the filtered measurment when getting the gyro position
    g_pos.y=get_single_gyro_pos(cfilter.y,gyro_rate.y);
    g_pos.z=get_single_gyro_pos(cfilter.z,gyro_rate.z);
    return g_pos;
  };
  
  
  
  
  ///////////////////////////////////////////////////////////
  //MAG (x,y vector to heading)
  float get_mag (void){
    Icart m=get_magnetic_vector();
    float head;
  
  
    head = -atan2(m.x,m.y) *RAD_TO_DEG;
    //print_Icart(m);
  
  
  
    //float magXcomp = m.x*cos(cfilter.x)-m.z*sin(cfilter.y);
    //Serial.print(magXcomp); Serial.print ("\t\t");
    //float magYcomp = m.x*sin(cfilter.x)*sin(cfilter.y)+m.y*cos(cfilter.x)-m.z*sin(cfilter.x)*cos(cfilter.y); // LSM9DS0
    //Serial.print(magYcomp); Serial.println ("\t\t");
    
    if(head < 0) head += 360; //this should yeild a 0 to 360 
    return head;
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

   
  
  
  ///////////////////////////////////////////////////////////
  //sensor to craft transform
  attitude sensor_to_craft(Fcart sensor){
    attitude res;
    
    res.roll= q_trans[0][0]*sensor.x+ q_trans[0][1]*sensor.y+ q_trans[0][2]*sensor.z;
    res.pitch= q_trans[1][0]*sensor.x+ q_trans[1][1]*sensor.y+ q_trans[1][2]*sensor.z;
    res.yaw= q_trans[2][0]*sensor.x+ q_trans[2][1]*sensor.y+ q_trans[2][2]*sensor.z;
    return res;
  };
  
  //////////////////////////////////////////////////
  //COMP FILTER
  void cf(float &val, float a, float g){
    
    //A= 1-G;
    val= g*G + a*A;
   
  };
  
  //////////////////////////////////////////////////
  //OFFSET CALIRATION 
  void calc_offsets(void){
  
    
    
    float mult=0.9;
    offset.roll=offset.roll*mult+cfilter.x*(1-mult);
    offset.pitch=offset.pitch*mult+cfilter.y*(1-mult);
    offset.yaw=offset.yaw*mult+cfilter.z*(1-mult);
    
  
    //mid_stick.roll=roll_in.high_time*0.5+mid_stick.roll*0.5;
    //mid_stick.pitch=pitch_in.high_time*0.5+mid_stick.pitch*0.5;
    //mid_stick.yaw=yaw_in.high_time*0.5+mid_stick.yaw*0.5;
    //mid_stick.throttle=throttle_in.high_time*0.5+mid_stick.throttle*0.5;
    
    //desired.roll=offset.roll;
    //desired.pitch=offset.pitch;
    //desired.yaw=offset.yaw;
     
  
    
  };
  

  

};



#endif

