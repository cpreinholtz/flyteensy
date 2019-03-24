#ifndef throttle_h
#define throttle_h


struct throttle_container  //type
{
  int fl;
  int fr;
  int bl;
  int br;
  
  void dbg(){
    Serial.print("fl: ");Serial.print(fl);Serial.print("\t\t");  
    Serial.print("fr: ");Serial.print(fr);Serial.print("\t\t");  
    Serial.print("bl: ");Serial.print(bl);Serial.print("\t\t");  
    Serial.print("br: ");Serial.print(br);Serial.print("\t\t");
    Serial.println("");
  };
};
typedef struct throttle_container Throttle;

#endif
