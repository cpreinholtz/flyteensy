#ifndef time_h
#define time_h


class timer{
public:
  timer(unsigned long msPeriod);
  void run();
  void setup();
  void dbg();
  void setPeriodS(unsigned long s);
  void setPeriodMs(unsigned long ms);
  unsigned long getEpoch(){return epoch;};
private:

  
  unsigned long periodMs;
  unsigned long startLoop;  
  unsigned long fastestLoop;
  unsigned long slowestLoop;
  unsigned long epoch;
};

#endif
