#ifndef __ANALOGLED__
#define __ANALOGLED__

#include "AnalogLight.h"

class AnalogLed: public AnalogLight {
public:
  AnalogLed(int pin);
  void setIntensity(int intensity);
private:
  int pin;
};

#endif
