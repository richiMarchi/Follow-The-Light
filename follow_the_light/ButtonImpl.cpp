#include "ButtonImpl.h"
#include "Arduino.h"

ButtonImpl::ButtonImpl(int pin){
  this->pin = pin;
  pinMode(pin, INPUT);
}

int ButtonImpl::standing(){
  return digitalRead(pin);
}
