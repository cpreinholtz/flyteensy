#ifndef pos_h
#define pos_h


struct fcartesian  //type
{
   float x;
   float y;
   float z;
   void dbg(){
     Serial.print("x: ");Serial.print(x);Serial.print("\t\t");  
     Serial.print("y: ");Serial.print(y);Serial.print("\t\t");  
     Serial.print("z: ");Serial.print(z);Serial.print("\t\t");  
     Serial.println("");
   };
};
typedef struct fcartesian Fcart; 

   
struct icartesian  //type
{
   int x;
   int y;
   int z;

   void dbg(){
    Serial.print("x: ");Serial.print(x);Serial.print("\t\t");  
    Serial.print("y: ");Serial.print(y);Serial.print("\t\t");  
    Serial.print("z: ");Serial.print(z);Serial.print("\t\t");  
    Serial.println("");
   };
};
typedef struct icartesian Icart;  

#endif


