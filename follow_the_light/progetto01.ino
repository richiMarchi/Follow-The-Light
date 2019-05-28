//Progetto di Riccardo Marchi, Marco Modigliani, Simone Venturi

#include <stdlib.h>
#include "Button.h"
#include "ButtonImpl.h"
#include "Light.h"
#include "Led.h"
#include "Potentiometer.h"
#include "AnalogLight.h"
#include "AnalogLed.h"

#define LED_PIN1 8
#define LED_PIN2 9
#define LED_PIN3 10
#define LED_FADE_BUTTON 11
#define BUTTON_PIN1 2
#define BUTTON_PIN2 4
#define BUTTON_PIN3 5
#define BUTTON_PIN 2
#define POT_PIN A3
#define MAXLEVEL 50

Light* led1;
Light* led2;
Light* led3;
AnalogLight* ledFading;
Button* button1;
Button* button2;
Button* button3;
Potentiometer* potentiometer;
int currIntensity = 0;
int fadeAmount = 5;
int level = 0;
int but1Stand = 0;
int but2Stand = 0;
int but3Stand = 0;
int score = 0;
int gameSpeed = 0;
int timeout = 0;
unsigned long mill = 0;
volatile bool start = true;
volatile bool finish = false;
volatile bool keepGoing = true;
int list[MAXLEVEL];

void setup() {
  led1 = new Led(LED_PIN1);
  led2 = new Led(LED_PIN2);
  led3 = new Led(LED_PIN3);
  button1 = new ButtonImpl(BUTTON_PIN1);
  button2 = new ButtonImpl(BUTTON_PIN2);
  button3 = new ButtonImpl(BUTTON_PIN3);
  ledFading = new AnalogLed(LED_FADE_BUTTON);
  potentiometer = new Potentiometer(POT_PIN);
  Serial.begin(9600);
}

void loop() { 
  if(start == true){
    EIFR |= (1 << INTF0);
    Serial.println("Welcome to Follow the Light!");
    Serial.println("Select difficulty from potentiometer(lower you choose, harder it is)");
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonInterrupt, RISING);
    fade();
    gameSpeed = map(potentiometer->readInput(),0,1023,0,255) + 200;
    Serial.print("You selected speed: ");
    Serial.println(gameSpeed);
    score = 0;
    detachInterrupt(0);
    Serial.println("Ready!");
  }
  ledFading -> setIntensity(0);
  delay(500);
  list[level] = getRand();
  
  showSequence();
  
  timeout = level * 1500;
  mill = millis();
  keepGoing = true;
  
  checkInputSequence();
  
  score = score + level;
  finish = false;
  delay(gameSpeed);
}






int getRand(){
  int ran = rand()%3 + 1;
  if(ran == 1){
    return LED_PIN1;
  } else if(ran == 2){
    return LED_PIN2;
  } else {
    return LED_PIN3;
  }
}

void fade(){
  while( start == true){
    ledFading -> setIntensity(currIntensity);
    currIntensity = currIntensity + fadeAmount;
    if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount ;
    }
    delay(20);
  }
}

void buttonInterrupt(){
  start = false;
}

void lost(){
  finish = true;
  start = true;
  level = 0;
  score = score * (456 - gameSpeed);
  Serial.print("Game Over - Score: ");
  Serial.println(score);
  ledFading -> setIntensity(255);
  delay(3000);
  ledFading -> setIntensity(0);
}

void showSequence() {
  for (int i = 0; i <= level; i++) {
    Light* led;
    switch(list[i]) {
      case LED_PIN1:{
        led = led1;
        break;
      }
      case LED_PIN2:{
        led = led2;
        break;
      }
      case LED_PIN3:{
        led = led3;
        break;
      }
    }
    delay(gameSpeed);
    led -> switchOn();
    delay(gameSpeed);
    led -> switchOff();
  }
  level++;
}

void checkInputSequence(){
  for(int i = 0; i < level; i++){
    keepGoing = true;
    while(!finish && keepGoing){
      but1Stand = button1 -> standing();
      but2Stand = button2 -> standing();
      but3Stand = button3 -> standing();
      checkTO();
      checkBut1(i);
      checkBut2(i);
      checkBut3(i);
    }
  }
}

void checkTO(){
  if(millis() - mill >= timeout){
    lost();
  }
}

void checkBut1(int spot){
  if(but1Stand == HIGH){
    delay(500);
    if(list[spot] == LED_PIN1){
      keepGoing = false;
    } else {
      lost();
    }
  }
}

void checkBut2(int spot){
  if(but2Stand == HIGH){
    delay(500);
    if(list[spot] == LED_PIN2){
      keepGoing = false;
    } else {
      lost();
    }
  }
}

void checkBut3(int spot){
  if(but3Stand == HIGH){
    delay(500);
    if(list[spot] == LED_PIN3){
      keepGoing = false;
    } else {
      lost();
    }
  }
}
