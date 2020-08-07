/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/IoTPa/Documents/Particle/EnumTest/src/EnumTest.ino"
/*
 * Project EnumTest
 * Description: Test passing enum to functions
 * Author: Brian Rashap
 * Date: 6-Aug-2020
 */
#include <EnumTest.h>

void setup();
void loop();
void printBday(BdayMonth bdm);
#line 9 "c:/Users/IoTPa/Documents/Particle/EnumTest/src/EnumTest.ino"
SYSTEM_MODE(SEMI_AUTOMATIC);    // Doesn't connect to wifi, needs Particle.connect() to activate wifi
//SYSTEM_MODE(AUTOMATIC);       // Automatically connect to Internet

BdayMonth bday;

void setup() {
  Serial.begin(9600);
  delay(500);          // Delay after Serial.begin() is ok, but not elsewhere :)
  bday = Apr;
}

void loop() {
  printBday(bday);
}

// This function prints my birthday month
void printBday(BdayMonth bdm) {
  switch (bdm) {
    case Jan:
      Serial.printf("My birthday is in January \n");
      break;
    case Feb:
      Serial.printf("My birthday is in February \n");
      break;
    case Mar:
      Serial.printf("My birthday is in March \n");
      break;
    case Apr:
      Serial.printf("My birthday is in April \n");
      break;
    default:
      Serial.printf("I am not sure when my birthday is \n");
      break;     
  }
}