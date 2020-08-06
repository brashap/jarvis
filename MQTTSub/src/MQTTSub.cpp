/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/IoTPa/Documents/jarvis/MQTTSub/src/MQTTSub.ino"
/*
 * Project MQTTSub
 * Description: Test Subscription to MQTT Feed from PlantWater project
 * Author:
 * Date:
 */

#include <Adafruit_MQTT.h>

#include "Adafruit_MQTT/Adafruit_MQTT.h" 
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h" 
#include "Adafruit_MQTT/Adafruit_MQTT.h" 

/************************* Adafruit.io Setup *********************************/ 
void setup();
void loop();
void MQTT_connect();
#line 15 "c:/Users/IoTPa/Documents/jarvis/MQTTSub/src/MQTTSub.ino"
#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883                   // use 8883 for SSL 
#define AIO_USERNAME  "rashap"
#define AIO_KEY       "598169fa3ba94636941a330976e42e2c"


/************ Global State (you don't need to change this!) ***   ***************/ 
TCPClient TheClient; 

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

/****************************** Feeds ***************************************/ 
// Setup Feeds to publish or subscribe 
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname> 

//Adafruit_MQTT_Publish TempC = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/TempC"); 

//Adafruit_MQTT_Subscribe TempF = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Home_Temperature"); 
Adafruit_MQTT_Subscribe roomTemp = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Home_Temperature"); 

/************Declare Variables*************/
unsigned int last;
float value;

void setup() {
  Serial.begin(9600);
  delay(100); //wait for Serial Monitor to startup

  // Setup MQTT subscription for onoff feed.
  //mqtt.subscribe(&TempF);
  mqtt.subscribe(&roomTemp);
  last = 0;
}

void loop() {
  MQTT_connect();

  if ((millis()-last)>120000) {
      Serial.printf("Pinging MQTT \n");
      if(! mqtt.ping()) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }

    // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here
 
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(10000))) {
    if (subscription == &roomTemp) {
      value = atof((char *)roomTemp.lastread);
      Serial.printf("Got: %0.2f \n",value);
    }
  }

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