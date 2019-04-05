#ifndef serial_tuner_h
#define serial_tuner_h


class serial_tuner{
public:
  serial_tuner(){};
  
  void run(){
    if(Serial.available() > 0) {
      char command = Serial.read();
      float val=Serial.parseFloat();
      Serial.flush();
      
      Serial.print("got command: \t:");
      Serial.print(command);
      Serial.print("\tvalue:\t");
      Serial.println(val);
      

      if      (command=='p') { k.kp=val; type=command;}
      else if (command=='i') { k.ki=val; type=command;}
      else if (command=='d') { k.kd=val; type=command;}
      available=true;
      k.dbg();
    }
  };

  
  void setup(PidConstants con){
    k.kp=con.kp;
    k.ki=con.ki;
    k.kd=con.kd;
    k.max=con.max;
    k.min=con.min;
    Serial.setTimeout(100);
  };

  PidConstants getConstants(){return k; available=false;};
  bool isAvailable(){return available;};
  void dbg();



private:
  PidConstants k;
  //float kp,ki,kd;
  char type;
  bool available=false;

};

#endif
