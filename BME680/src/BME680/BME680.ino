/*
 * Project BME680 and Thingspace Project
 * Description: BME680 Temp, Pressure, Humidity, and Air Quality
 *              Sent to the Particle Cloud and published to Thingspace
 * Author: Brian Rashap
 * Date: 10/23/2019
 */

#include "Particle.h"
#include "Adafruit_Sensor.h"
// #include "Adafruit_BME280.h"
#include "Adafruit_BME680.h"


#define SEALEVELPRESSURE_HPA (1013.25)

// Adafruit_BME280 bme; // I2C
Adafruit_BME680 bme; // I2C

unsigned long delayTime;
double tempC;
float humid;

void setup() {
    Wire.begin();

    Particle.variable("temp", tempC);

    Serial.begin(9600);
    while(!Serial);    // time to get serial running
    Serial.println("BME680 test");

    unsigned status;

    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin();
    if (!status) {
      Serial.println("Could not find a valid BME680 sensor, check wiring!");
      while (1);
    }

    // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms

    delayTime = 30000;
}

void loop()
{
    
    printValues();
    String temp = String(tempC);
    Particle.publish("temp", temp, PRIVATE);
    delay(delayTime);
}

double printValues()
{
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    tempC = bme.readTemperature();

    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    humid = bme.readHumidity();

    Serial.print("Gas = ");
    Serial.print(bme.gas_resistance / 1000.0);
    Serial.println(" KOhms");

    Serial.println();

    return tempC; 
}