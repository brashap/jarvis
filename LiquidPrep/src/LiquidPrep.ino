/*
 * Project LiquidPrep
 * Description: IBM Call for Coders Liquid Prep soil moisture monitorning test program
 * Author: Brian Rashap
 * Date: 3-NOV-2020
 */

#include "Credentials.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_SSD1306.h"

// Setup SSD_1306 Display
#define OLED_ADDR   0x3C
#define SSD1306_128_64
Adafruit_SSD1306 display(-1);

// THese #include statement is for MQTT
#include "Adafruit_MQTT/Adafruit_MQTT.h" 
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h" 
#include "Adafruit_MQTT/Adafruit_MQTT.h" 

/************ Global State (you don't need to change this!) ***   ***************/ 
TCPClient TheClient; 

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

/****************************** Feeds ***************************************/ 
Adafruit_MQTT_Publish Moisture = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Africa_Moisture");

/************Declare Variables*************/
int soilPin = A2;
int moist;
int soilDelay = 60000;

int lastTime;
int last;
const int updateTime = 600000; 

void setup() {
  Serial.begin(9600);

  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
  printhello();  
  pinMode(soilPin,INPUT);
  lastTime = -updateTime;
  MQTT_connect();
  delay(5000);
}

void loop() {

  if ((millis()-last)>120000) {
    Serial.printf("Pinging MQTT \n");
    if(! mqtt.ping()) {
      Serial.printf("Disconnecting \n");
      mqtt.disconnect();
    }
    last = millis();
  }

  if((millis()-lastTime > updateTime)) {
    MQTT_connect();
    moist = analogRead(soilPin);
    printMoist(moist);

    //Publish to Adafruit.io
    if(mqtt.Update()) {
      Serial.printf("Publishing Moisture Data: %i \n",moist);
      Moisture.publish(moist);
    } 

    lastTime=millis();
  }
  delay(1);
}



void printMoist(int moistVal) {

  Serial.printf("The moisture reading is %i \n",moist);
  display.clearDisplay();

  display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 63, WHITE);
  display.drawPixel(127, 63, WHITE);

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,10);

  display.printf(" MOISTURE \n   %i \n",moist);
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

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
 
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}