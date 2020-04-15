/*
 * Project AdaControl
 * Description: Sample code to publish and subscribe
 *              to Adafruit IO cloud
 * Author: Brian A Rashap
 * Date: 14-Mar-2019
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

Adafruit_MQTT_Publish state1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/statex");
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/statesend"); 

#include "Particle.h"


int ledPin = A0;
int buttonPin = D0;
float led;
int bright;

long time1;
long time2;


// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
Serial.begin(115200); 
   while(!Serial);    // time to get serial running

pinMode(ledPin,OUTPUT);
pinMode(buttonPin,INPUT_PULLDOWN);
bright = 255;

Serial.println("Let's get going");

   // Setup MQTT subscription for onoff feed. 
   mqtt.subscribe(&onoffbutton); 


}

// loop() runs over and over again, as quickly as it can execute.
void loop() {

Serial.print("Start Loop   ");
Serial.println(millis());
Serial.print("Button State   ");
Serial.println(digitalRead(buttonPin));
  if (digitalRead(buttonPin) == HIGH) {
    analogWrite(ledPin,bright);
    led = 1.0;
  }
  else {
    analogWrite(ledPin,0);
    led = 0.0;
  }

delay(3000);   // throttle Adafruit.IO

Serial.print("Start Publish   ");
Serial.println(millis());
Serial.println(mqtt.Update());
state1.publish(led);
Serial.print("End Publish   ");
Serial.println(millis());

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
      bright = atoi((char *)onoffbutton.lastread);
      Serial.print("---- Integer ---- ");
      Serial.println(bright);
    }
  }

Serial.print("End Subscribe   ");
Serial.println(millis());  

  time1=millis();
  time2=millis();
  while((time2-time1)<2000) {
    time2=millis();
  }



//if(mqtt.Update()) {
//  state1.publish(led);
//}
}