/*
 * Project:     NCD0x50
 * Description: Using the NCD I2C mini modules
 *              More info found at ControlEverything.com
 * Author:      Brian Rashap
 * Date:        9-Jan-2020
 */

#include <Wire.h>
#include <application.h>
#include <spark_wiring_i2c.h>

// ADC121C_MQ131 I2C address is 0x50(80)
#define Addr1 0x50   // Ozone Sensor
#define Addr3 0x52   // CO2 Sensor

int raw_adc = 0;
double ppm = 0.0;

void setup()
{
  // Set variable
  Particle.variable("i2cdevice131", "ADC121C_MQ131");
  Particle.variable("i2cdevice9", "ADC121C_MQ9");
  Particle.variable("PPM", ppm);

  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  delay(300);
}

void loop()
{
  oz();
  co2();
  delay(10000);
}

void oz() {
    unsigned int data[2];

  // Start I2C transmission
  Wire.beginTransmission(Addr1);
  // Select data register
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(Addr1, 2);

  // Read 2 bytes of data
  // raw_adc msb, raw_adc lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data to 12-bits
  raw_adc = ((data[0] & 0x0F) * 256) + data[1];
  ppm = (1.99 * raw_adc) / 4095.0 + 0.01;

  // Output data to dashboard
  //Particle.publish("Ozone Concentration : ", String(ppm) + " ppm");
  Particle.publish("Ozone", String(ppm));
  Serial.print("Ozone Concenration (ppm): ");
  Serial.println(ppm);
}

void co2()
{
  unsigned int data[2];

  // Start I2C transmission
  Wire.beginTransmission(Addr3);
  // Select data register
  Wire.write(0x00);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(Addr3, 2);

  // Read 2 bytes of data
  // raw_adc msb, raw_adc lsb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
  delay(300);

  // Convert the data to 12-bits
  raw_adc = ((data[0] & 0x0F) * 256) + data[1];
  ppm = (1000 / 4096.0) * raw_adc + 10;

  // Output data to dashboard
  // Particle.publish("Carbon Monoxide Concentration : ", String(ppm) + " ppm");
  Particle.publish("CO2", String(ppm));
  Serial.print("CO2 Concenration (ppm): ");
  Serial.println(ppm);
}