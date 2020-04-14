/*
 * Project PlantWater
 * Description:
 * Author:
 * Date:
 */
#include <Particle.h>
#include <JsonParserGeneratorRK.h>

int soilPin = A2;
int moist;
int soilDelay = 60000;

int threshold = 3500;
int pumpPin = D4;
int waterTime = 2500;
bool watered;

int tempPin = A3;
int temp;

char currentTime[25];
char current[9];
String t;
String t2;

void setup() {
  Serial.begin(9600);
  pinMode(soilPin,INPUT);
  pinMode(pumpPin,OUTPUT);
  Time.zone(-6); // set timezone to MDT
  Particle.syncTime(); 
  Particle.variable("Moisture", moist);
  Particle.variable("Temperature", temp);
  Particle.variable("Plant Watered", watered);
}

void loop() {
  moist = analogRead(soilPin);
  watered = waterPlant(moist);

  temp = analogRead(tempPin);
  printMoist(moist);
  Particle.publish("Moisture", String(moist),PRIVATE);
  Particle.publish("Temperature", String(temp),PRIVATE);
  Particle.publish("Plant Watered", String(watered),PRIVATE);
  createEventPayLoad(moist,temp,watered);

  delay(soilDelay);
}

bool waterPlant(int moistVal) {
  if(moistVal > threshold) {
    digitalWrite(pumpPin,HIGH);
    delay(waterTime);
    digitalWrite(pumpPin,LOW);
    return true;
  }
  else
  {
    return false;
  }
    
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

void createEventPayLoad(int moistValue, int tempValue, bool waterED) {
  JsonWriterStatic<256> jw;
  {
    JsonWriterAutoObject obj(&jw);

    jw.insertKeyValue("Moisture", moistValue);
    jw.insertKeyValue("Temperature", tempValue);
    jw.insertKeyValue("Plant Watered", waterED);
  }
  Particle.publish("env-vals",jw.getBuffer(), PRIVATE);
}