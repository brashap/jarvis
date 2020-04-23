/*
 * Project HomeWeather
 * Description:
 * Author:
 * Date:
 */

#include <Particle.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BME680.h"
#include <JsonParserGeneratorRK.h>

// Adafruit_BME280 bme; // I2C
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME680 bme; // I2C
unsigned status;

/******Variable Declarations*********/
float tempF;
float pres;
float humid;
float gas;
int pm25; 
int delayTime = 30*60*1000;

// setup() runs once, when the device is first turned on.
void setup() {
  Wire.begin();

  Serial.begin(9600);
  delay(1000);    // time to get serial running
  Serial.println("BME680 test");
  
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
    }
  else {
    Serial.println("BME680 Is Online and Ready!!!");
  }

    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  tempF = (bme.readTemperature()*(9.0/5.0))+32;
  pres = ((bme.readPressure() / 100.0F)*0.02953)+5;
  humid = bme.readHumidity();
  gas = bme.gas_resistance / 1000.0;
  pm25 = 13; //don't forget to change this.
  
  Serial.printlnf("Temperature(F) = %1.2f",tempF);
  Serial.printlnf("Pressure(inHg) = %1.2f",pres);

  printValues();

   createEventPayLoad(tempF,pres,humid,gas,pm25);

  delay(delayTime);
}

void createEventPayLoad(float tempValue, float presValue, float humidValue, float gasValue, int pm25Value) {
  JsonWriterStatic<256> jw;
  {
    JsonWriterAutoObject obj(&jw);

    jw.insertKeyValue("Home_Temp", tempValue);
    jw.insertKeyValue("Home_Pres", presValue);
    jw.insertKeyValue("Home_Humid", humidValue);
    jw.insertKeyValue("Home_Gas", gasValue);
    jw.insertKeyValue("Home_PM25", pm25Value);
  }
  Particle.publish("bme-vals",jw.getBuffer(), PRIVATE);
}

void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.print("Gas = ");
    Serial.print(bme.gas_resistance / 1000.0);
    Serial.println(" KOhms");

    Serial.println();  
}