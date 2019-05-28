#include "AnalogLed.h"
#include "Arduino.h"

AnalogLed::AnalogLed(int pin){
  this->pin = pin;
  pinMode(pin,OUTPUT);
}

void AnalogLed::setIntensity(int intensity){
  analogWrite(pin,intensity);
}
