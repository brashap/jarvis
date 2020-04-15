/*
 * Project BME280_AdafruitIO
 * Description: Trial Script to send Accelerometer Data to Adafruit IO cloud
 * Author: Brian Rashap
 * Date: 11/7/2019
 */

#include <Adafruit_MQTT.h>

// This #include statement was automatically added by the Particle IDE. 
#include "Adafruit_MQTT/Adafruit_MQTT.h" 
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h" 
#include "Adafruit_MQTT/Adafruit_MQTT.h" 


/************************* Adafruit.io Setup *********************************/ 
#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883                   // use 8883 for SSL 
#define AIO_USERNAME  "rashap"
#define AIO_KEY       "598169fa3ba94636941a330976e42e2c"


/************ Global State (you don't need to change this!) ***   ***************/ 
TCPClient TheClient; 

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 


/****************************** Feeds ***************************************/ 
// Setup a feed called 'voltage' for publishing. 
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname> 

Adafruit_MQTT_Publish temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish press = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Pressure");
Adafruit_MQTT_Publish rh = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Relative_Humidity"); 

/*************************** Setup BME280 ***********************************/
#include "Particle.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

Adafruit_BME280 bme; // I2C

unsigned long delayTime;
#define SEALEVELPRESSURE_HPA (1013.25)

/*************************** Sketch Code ************************************/ 
float temperature;
float pressure;
float humidity;

void setup()  
{ 
   Wire.begin();
   
   Serial.begin(115200); 
   while(!Serial);    // time to get serial running
   pinMode(A0,INPUT);
   Serial.println("\nI2C Scanner"); 

	    unsigned status;
    
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;
    I2CScan();    
} 

void loop() 
{ 
   printValues();
       temperature = bme.readTemperature()*1.6+32.0;
       // pressure = (bme.readPressure() / 100.0) / 68.9476;   //Engish Units
       pressure = (bme.readPressure() / 100.0);             //SI Units
       humidity = bme.readHumidity();
       Serial.println("-------------Data to Publish---------------");
       Serial.println(temperature);
       Serial.println(pressure);
       Serial.println(humidity);
       Serial.println("---------------End Publish-----------------");
 if( mqtt.Update() ){
       temp.publish(temperature); 
       press.publish(pressure);
       rh.publish(humidity);
 } 
   delay(10000); 
}

void I2CScan()
{
	byte error, address;
	int nDevices;

   Serial.println("---------------------------");
	Serial.println("Scanning for I2C Devices...");
   Serial.println("---------------------------");
	delay(5000);

	nDevices = 0;
	for(address = 1; address < 127; address++ )
	{
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == 0)
		{
			Serial.print("I2C device found at address 0x");
			if (address<16)
				Serial.print("0");
			Serial.print(address,HEX);
			Serial.println("  !");

			nDevices++;
		}
		else if (error==4)
		{
			Serial.print("Unknow error at address 0x");
			if (address<16)
				Serial.print("0");
			Serial.println(address,HEX);
		}
	}
	if (nDevices == 0)
		Serial.println("No I2C devices found\n");
	else
		Serial.println("done\n");
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

    Serial.println();
}