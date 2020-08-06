/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/IoTPa/Documents/jarvis/PlantWater/src/PlantWater.ino"
/*
 * Project PlantWater
 * Description:
 * Author:
 * Date:
 */

#include <JsonParserGeneratorRK.h>
#include <Adafruit_MQTT.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// Setup OLED Display
void setup();
void loop();
int waterPlant(int moistVal);
void printMoist(int moistVal);
void createEventPayLoad(int moistValue, float tempValue, float presValue, float humValue, int waterED);
void printValues();
float getDust();
void oledprint(float Otemp, float Opres, float Ohum, float Odust, int Omoist);
void printhello();
#line 14 "c:/Users/IoTPa/Documents/jarvis/PlantWater/src/PlantWater.ino"
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

/*=========================================================================
    SSD1306 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.

    SSD1306_128_64  128x64 pixel display

    SSD1306_128_32  128x32 pixel display

    SSD1306_96_16

    -----------------------------------------------------------------------*/
   #define SSD1306_128_64
//   #define SSD1306_128_32
//   #define SSD1306_96_16

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
Adafruit_MQTT_Publish Hdust = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Home_Dust"); 

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

int threshold = 2425;
int pumpPin = D4; 
int waterTime = 500;
int watered;
//change this
int tempPin = A3;
double temp;
double pres;
double hum;
double alt; 

float dust;

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

  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  printhello();

}

void loop() {
  moist = analogRead(soilPin);
  watered = waterPlant(moist);
  temp = (bme.readTemperature()*(9.0/5.0))+32;
  pres = (bme.readPressure() / 100.0F * 0.02953)+5;
  hum = bme.readHumidity();
  dust = getDust();
  Serial.printf("Dust Value = %0.2f \n",dust);
  printMoist(moist);
  printValues();

    if(mqtt.Update()) {
       Htemp.publish(temp); 
       Hpres.publish(pres);
       Hhum.publish(hum);
       Hmoist.publish(moist);
       Hwater.publish(watered);
       Hdust.publish(dust);
    } 

    oledprint(temp, pres, hum, dust, moist);

  Particle.publish("Moisture", String(moist),PRIVATE);
  Particle.publish("Temperature", String(temp),PRIVATE);
  Particle.publish("Plant Watered", String(watered),PRIVATE);
  
  createEventPayLoad(moist,temp,pres,hum,watered);

    for(i=0;i<60;i++) {
      Adafruit_MQTT_Subscribe *subscription;
      Serial.printf("x%i ",i);
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

int waterPlant(int moistVal) {
  if(moistVal > threshold) {
    Serial.printlnf("The %i > %i, turning on pump", moist, threshold);
    delay(1000);
    digitalWrite(pumpPin,HIGH);
    delay(waterTime);
    digitalWrite(pumpPin,LOW);
    return 1;
  }
  else
  {
    Serial.printlnf("The %i < %i no water needed", moist, threshold);
    return 0;
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

void createEventPayLoad(int moistValue, float tempValue, float presValue, float humValue, int waterED) {
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

float getDust() {
  
  int pinDust = 8;
  unsigned long duration;
  unsigned long starttime;
  unsigned long sampletime_ms = 30000;//sampe 30s ;
  unsigned long lowpulseoccupancy = 0;
  float ratio = 0;
  float concentration = -1;

  pinMode(pinDust,INPUT);
  starttime = millis();//get the current time;

  while(concentration == -1) {
    duration = pulseIn(pinDust, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;

    if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
    {
        ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
        concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
        Serial.print(lowpulseoccupancy);
        Serial.print(",");
        Serial.print(ratio);
        Serial.print(",");
        Serial.println(concentration);
        lowpulseoccupancy = 0;
        starttime = millis();
    }
  }
return concentration;
}

void oledprint(float Otemp, float Opres, float Ohum, float Odust, int Omoist) {

   // initialize and clear display
  display.clearDisplay();
  display.display();

  // display a pixel in each corner of the screen
  display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 63, WHITE);
  display.drawPixel(127, 63, WHITE);

  // display a line of text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,5);
  display.printf("Environmental Reading");
  display.setCursor(0,18);
  display.printf("Temperature(F) %0.2f\n",Otemp);
  display.printf("Pressure (hPa) %0.2f\n",Opres);
  display.printf("Humidity (%rH)  %0.2f\n",Ohum);
  display.printf("Dust Level: %0.2f \n",Odust);
  display.printf("Moisture: %d \n",Omoist);
  display.display();
}

void printhello() {
  display.clearDisplay();
  // display a pixel in each corner of the screen
  display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 63, WHITE);
  display.drawPixel(127, 63, WHITE);
  // display a line of text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(27,30);
  display.print("Hello, world!");
  display.display();
}