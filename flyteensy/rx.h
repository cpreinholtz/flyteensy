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
    
    void run(){//get last desired
        // look for a good SBUS packet from the receiver
      if(x8r.read(&channels[0], &failSafe, &lostFrame)){
        updateDesired();
    

      }
    };

    void setup(){

    };

    
    
    void dbg(){



        
        desired.dbg();
    };

    void printRaw(){
        int i;
        for (i=0;i<16; i++){
          Serial.print(channels[i]);
          Serial.print("\t");
        }
        Serial.println("\t");
    };

    void updateDesired(){
      desired.roll=map(channels[rch], fromLow, fromHigh, toLow, toHigh);
      desired.pitch=map(channels[pch], fromLow, fromHigh, toLow, toHigh);
      desired.yaw=map(channels[ych], fromLow, fromHigh, toLow, toHigh);
      desired.throttle=channels[tch];
      aux=channels[ach];
      mode=channels[mch];
    };

  private:
  
    
    


    ///////////////////////////////////

    attitude desired;
    uint16_t aux;
    uint16_t mode;

    uint16_t channels[16];
    bool failSafe;
    bool lostFrame;
    
    
    static const uint16_t rch= 0;
    static const uint16_t pch= 1;
    static const uint16_t tch= 2;
    static const uint16_t ych= 3;
    static const uint16_t ach= 4;
    static const uint16_t mch= 5;
    
    
    static const int mapCenter=1024;//unused   
    static const int auxLow=600;//lessthan
    static const int auxHigh=1400;//lessthan
    
    //maps
    static const int fromLow=240;
    static const int fromHigh=1807;
    static const int toLow=-255;
    static const int toHigh=255;

    

};

#endif