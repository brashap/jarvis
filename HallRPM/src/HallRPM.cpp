/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/IoTPa/Documents/jarvis/HallRPM/src/HallRPM.ino"
/*
 * Project HallRPM
 * Description: Using Hall Effect Sensor to measure PRM on rotary show tools.
 * Author: Brian Rashap
 * Date: 21-Nov-2020
 */

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

void setup();
void loop();
bool lowToHigh(int inputPin);
void helloWorld();
void displayRPM(float rpm);
#line 11 "c:/Users/IoTPa/Documents/jarvis/HallRPM/src/HallRPM.ino"
#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);

const int hallPin=D2;
unsigned int startTime, endTime, elapseTime;
float RPM;

SYSTEM_THREAD(ENABLED); // allow DFU mode even with below WHILE LOOP

void setup() {
  Serial.begin(9600);

  // OLED Display initialization
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	helloWorld();
  pinMode(hallPin,INPUT);
}

void loop() {
  startTime = 0;
  endTime = 0;
  while (endTime == 0) {
    if(lowToHigh(hallPin) == true) {
      if(startTime == 0) {
        startTime = micros();
      }
      else {
        endTime = micros();
      }
    }
  }
  elapseTime = endTime - startTime;
  RPM = 60000000.0 / elapseTime;
  Serial.printf("RPM = %0.2f \n", RPM);
  displayRPM(RPM);
}

// Returns a true when hall sensor goes from Low to High
bool lowToHigh(int inputPin) {
  static bool lastState = HIGH;
  bool currentState;
  bool trigger=false;

  currentState = digitalRead(inputPin);
  if (currentState == !lastState) {
    if (currentState == HIGH) {
      trigger = true;
    }
  lastState = currentState;
  } 
  return trigger;
}

void helloWorld() {
	display.clearDisplay();
	display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,15);
  display.printf("   Hello\n   World");
	display.display();
}

void displayRPM(float rpm) {
	display.clearDisplay();
	display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,5);
  display.printf("    RPM \n\n  %6.2f \n",rpm);
	display.display();
}