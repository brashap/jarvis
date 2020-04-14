/*
 * Project PlantWater
 * Description:
 * Author:
 * Date:
 */

int soilPin = A2;
int moist;
int soilDelay = 10000;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
Serial.begin(9600);
pinMode(soilPin,INPUT);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
moist = analogRead(soilPin);
Serial.printf("The moisture reading is %i \n",moist);
delay(soilDelay);
}