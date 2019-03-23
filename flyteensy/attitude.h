


struct attitude  //type
{
   float roll;
   float pitch;
   float yaw;
   float throttle;

  void dbg(){
    Serial.print("roll: ");Serial.print(roll);Serial.print("\t\t");
    Serial.print("pitch: ");Serial.print(pitch);Serial.print("\t\t");
    Serial.print("yaw: ");Serial.print(yaw);Serial.print("\t\t");
    Serial.print("throt: ");Serial.print(throttle);Serial.print("\t\t");
    Serial.println("");
  };

   
};



