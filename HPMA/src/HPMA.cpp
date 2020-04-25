/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/IoTPa/Documents/jarvis/HPMA/src/HPMA.ino"
/*
 * Project HPMA
 * Description:
 * Author:
 * Date:
 */

#include <HPMA115S0.h>
//#include <SoftwareSerial.h>

//Create an instance of the hpma115S0 library
void setup();
void loop();
#line 12 "c:/Users/IoTPa/Documents/jarvis/HPMA/src/HPMA.ino"
HPMA115S0 hpma115S0(Serial1);

void setup() {
  Serial.begin(57600);
  Serial1.begin(9600);
  delay(5000);
  Serial.println("Starting...");
  hpma115S0.Init();
  hpma115S0.StartParticleMeasurement();
}

void loop() {
  unsigned int pm2_5, pm10;
  if (hpma115S0.ReadParticleMeasurement(&pm2_5, &pm10)) {
    Serial.println("PM 2.5: " + String(pm2_5) + " ug/m3" );
    Serial.println("PM 10: " + String(pm10) + " ug/m3" );
  }
  delay(1000);
}