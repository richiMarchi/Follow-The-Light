#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__

class Potentiometer {
public:
  Potentiometer(int pint);
  int readInput();
private:
  int pin;
};

#endif
