/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/IoTPa/Documents/Brian/Particle/TwoLoops/src/TwoLoops.ino"
/*
 * Project TimeLoops
 * Description: How to have loops run a different times
 * Author: Brian Rashap
 * Date: 16-APR-2020
 */
void setup();
void loop();
#line 7 "c:/Users/IoTPa/Documents/Brian/Particle/TwoLoops/src/TwoLoops.ino"
int lastSecond;
int lastMinute;
int lastHour;
int currentTime;

void setup() {
  lastSecond =  -1000;  // set to negative one second
  lastMinute = -60000;  // set to negative one minutes
  lastHour =  -360000;  // set to negative one hour
}

void loop() {
  //run constantly
  currentTime = millis();

  //run once per second
  if((currentTime-lastSecond)>1000) {
    Serial.print(".");
    lastSecond = millis();
  }  

  //run once per minute
  if((currentTime-lastMinute)>60000) {
    Serial.println();
    Serial.println("Minute");
    lastMinute = millis();
  }

  //run once per hour
  if((currentTime-lastHour)>360000) {
    Serial.println();
    Serial.println("Hour");
    lastHour = millis();
  }
}