/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "c:/Users/IoTPa/Documents/jarvis/PlantWater/src/PlantWater.ino"
/*
 * Project PlantWater
 * Description:
 * Author:
 * Date:
 */
#include <Particle.h>
#include <JsonParserGeneratorRK.h>


void setup();
void loop();
void printMoist(int moistVal);
void createEventPayLoad(int moistValue, int tempValue);
#line 11 "c:/Users/IoTPa/Documents/jarvis/PlantWater/src/PlantWater.ino"
int soilPin = A2;
int moist;
int soilDelay = 10000;

int tempPin = A3;
int temp;

char currentTime[25];
char current[9];
String t;
String t2;

void setup() {
  Serial.begin(9600);
  pinMode(soilPin,INPUT);
  Time.zone(-6); // set timezone to MDT
  Particle.syncTime(); 
  Particle.variable("Moisture", moist);
  Particle.variable("Temperature", temp);
}

void loop() {
  moist = analogRead(soilPin);
  temp = analogRead(tempPin);
  printMoist(moist);
  Particle.publish("Moisture", String(moist),PRIVATE);
  Particle.publish("Temperature", String(temp),PRIVATE);
  createEventPayLoad(moist,temp);
  delay(soilDelay);
}

void printMoist(int moistVal) {
  t = Time.timeStr(); 
  t.toCharArray(currentTime,25);
  t2 = t.substring(11,19);
  t2.toCharArray(current,9);
  Serial.println(t2);
  Serial.printf("At %s the moisture reading is %i \n",currentTime,moist);
  //Serial.printf("At %s the moisture reading is %i \n",Time.timeStr().c_str(),moist);
  Serial.printf("The time is %s \n",current);
}

void createEventPayLoad(int moistValue, int tempValue) {
  JsonWriterStatic<256> jw;
  {
    JsonWriterAutoObject obj(&jw);

    jw.insertKeyValue("Moisture", moistValue);
    jw.insertKeyValue("Temperature", tempValue);
  }
  Particle.publish("env-vals",jw.getBuffer(), PRIVATE);
}