#ifndef rx_h
#define rx_h

#include "Arduino.h"
#include "elapsedMillis.h"
#include "attitude.h"
#include "SBUS.h"

//class HardwareSerial;

extern SBUS x8r;

class rx{
  
  public:
    rx(){};//ctor
    
    void run(bool Cal){//get last desired
        // look for a good SBUS packet from the receiver
      if(x8r.read(&channels[0], &failSafe, &lostFrame)){
        updateDesired(Cal); 
      }
    };

    void setup(){

    };//ctor

    
    
    void dbg(){
        desired.dbg();
    };//dbg

    void printRaw(){
        int i;
        for (i=0;i<16; i++){
          Serial.print(channels[i]);
          Serial.print("\t");
        }
        Serial.println("\t");
    };//print raw

    void updateDesired(bool Cal){
      desired.roll=map(channels[rch], fromLow, fromHigh, toLow, toHigh);
      desired.pitch=map(channels[pch], fromLow, fromHigh, toLow, toHigh);
      desired.yaw=map(channels[ych], fromLow, fromHigh, toLow, toHigh);      
      if (!Cal) desired.throttle=map(channels[tch], fromCenter, fromHigh, motor_min, motor_max);
      else desired.throttle=map(channels[tch], fromLow, fromHigh, motor_min, motor_max);   //dbg only (full range)

      updateAux();
      
      //aux=channels[ach];
      //mode=channels[mch];
    };//update desired



    void updateAux(){
      if (channels[swcCh]<auxLow)         mode=idle;    //idle=up 
      else if (channels[swcCh]>auxHigh)   mode=fly;     //flight=down
      else                                mode=offset;  //offset=middle
      
      if (channels[swbCh]<auxLow)         tune=up;      //up no dbg prints
      else if (channels[swbCh]>auxHigh)   {tune=down;   desired.throttle=motor_hover;} //down if(mode=fly) set throttle to baseline hover
      else                                tune=middle;  //middle cleat ki and print

        
    };//update aux
    
    //enums for modes and aux channels
    enum Mode{
      idle=1,
      offset,
      fly   
    };    
    
    enum Tune{
      up=1,
      down,
      middle   
    }; 
    
    //getters
    attitude getDesired(){ return desired;};
    float getDesiredThrottle(){ return desired.throttle;};
    
    Mode getMode(){return mode;};
    Tune getTune(){return tune;};
 
    

  private:
  
    
    


    ///////////////////////////////////

    attitude desired;
    

    
    Tune tune=up;
    Mode mode=idle;
    
    uint16_t channels[16];
    bool failSafe;
    bool lostFrame;
    
    
    static const uint16_t rch= 0;
    static const uint16_t pch= 1;
    static const uint16_t tch= 2;
    static const uint16_t ych= 3;
    static const uint16_t swcCh= 4;
    static const uint16_t swbCh= 5;
    
    
      


    
    float motor_min= 680;//uSeconds
    float motor_start= 821;//blades start turning
    float motor_max= 1400;
    float motor_hover= 1150;
    
    //maps
    static const int fromCenter=1024;
    static const int fromLow=240;
    static const int fromHigh=1807;
    static const int toLow=-50;
    static const int toHigh=50;

    static const int auxLow =fromLow+300;
    static const int auxHigh =fromHigh-300;
    

};

#endif
