/*
 * Project PlantWater
 * Description:
 * Author:
 * Date:
 */
//#include <Particle.h>
#include <JsonParserGeneratorRK.h>

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

Adafruit_MQTT_Publish Htemp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Home_Temperature");
Adafruit_MQTT_Publish Hmoist = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Home_Moisture");
Adafruit_MQTT_Publish Hwater = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Home_Water"); 
Adafruit_MQTT_Publish Hpres = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Home_Pressure"); 
Adafruit_MQTT_Publish Hhum = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Home_Humidity"); 

Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LED_On"); 

/***************SetUp BME280*****************/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C

/************Declare Variables*************/
int soilPin = A2;
int moist;
int soilDelay = 60000;

int threshold = 2700;
int pumpPin = D4;
int waterTime = 1250;
bool watered;
//change this
int tempPin = A3;
double temp;
double pres;
double hum;
double alt;

unsigned status;

char currentTime[25];
char current[9];
String t;
String t2;

long time1;
long time2;
int button;
int i;

int delayStart;

void setup() {
  Serial.begin(9600);
  pinMode(soilPin,INPUT);
  pinMode(pumpPin,OUTPUT);
  pinMode(D7,OUTPUT);
  digitalWrite(pumpPin,LOW);
  Time.zone(-6); // set timezone to MDT
  Particle.syncTime(); 
  Particle.variable("Moisture", moist);
  Particle.variable("Temperature", temp);
  Particle.variable("Plant Watered", watered);

    // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);

// Initialize BME280
    Serial.println(F("BME280 test"));
    
    status = bme.begin(0x76);
    if (!status)
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x");
        Serial.println(bme.sensorID(), 16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1)
            ;
    }

}

void loop() {
  moist = analogRead(soilPin);
  watered = waterPlant(moist);

  temp = (bme.readTemperature()*(9.0/5.0))+32;
  pres = bme.readPressure() / 100.0F;
  hum = bme.readHumidity();
  printMoist(moist);
  printValues();

    if(mqtt.Update()) {
       Htemp.publish(temp); 
       Hmoist.publish(moist);
       Hwater.publish(watered);
    } 

  Particle.publish("Moisture", String(moist),PRIVATE);
  Particle.publish("Temperature", String(temp),PRIVATE);
  Particle.publish("Plant Watered", String(watered),PRIVATE);
  
  createEventPayLoad(moist,temp,pres,hum,watered);

    for(i=0;i<10;i++) {
      Adafruit_MQTT_Subscribe *subscription;
        while ((subscription = mqtt.readSubscription(10000))) {  // do this loop for 10 seconds
          if (subscription == &onoffbutton) {
            button = atoi((char *)onoffbutton.lastread);   //convert adafruit string to int
            Serial.printf("Button State is %i \n",button);
            if(button==1) {
              digitalWrite(D7,1);
              watered = waterPlant(3000);
                  if(mqtt.Update()) {
                    Htemp.publish(temp); 
                    Hmoist.publish(moist);
                    Hwater.publish(watered);
                  } 
              digitalWrite(D7,0);
            }
          }
        }
    }    
}

bool waterPlant(int moistVal) {
  if(moistVal > threshold) {
    Serial.printlnf("The %i > %i, turning on pump", moist, threshold);
    delay(1000);
    digitalWrite(pumpPin,HIGH);
    delay(waterTime);
    digitalWrite(pumpPin,LOW);
    return true;
  }
  else
  {
    Serial.printlnf("The %i < %i no water needed", moist, threshold);
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

void createEventPayLoad(int moistValue, float tempValue, float presValue, float humValue, bool waterED) {
  JsonWriterStatic<256> jw;
  {
    JsonWriterAutoObject obj(&jw);

    jw.insertKeyValue("Moisture", moistValue);
    jw.insertKeyValue("Temperature", tempValue);
    jw.insertKeyValue("Pressure", presValue);
    jw.insertKeyValue("Humidity", humValue);
    jw.insertKeyValue("Plant Watered", waterED);
  }
  Particle.publish("env-vals",jw.getBuffer(), PRIVATE);
}

void printValues()
{
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