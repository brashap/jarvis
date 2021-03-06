/*
 * Project BoronGPS
 * Description: Trial Program to Use TinyGPS++ on the Particle Boron with Display to OLED
 * Author: Brian Rashap
 * Date: 19-NOV-2020
 */

#include "Particle.h"
#include "TinyGPS++.h"
#include <JsonParserGeneratorRK.h>

/*
 * GPS Module Connections
 * GPS Unit Rx connects to Argon Pin D9  (UART1_TX)
 * GPS Unit Tx connects to Argon Pin D10 (UART1_RX)
*/

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);

SYSTEM_THREAD(ENABLED);
//SYSTEM_MODE(SEMI_AUTOMATIC);

const unsigned long PUBLISH_PERIOD = 120000;
const unsigned long SERIAL_PERIOD = 5000;
const unsigned long MAX_GPS_AGE_MS = 10000; // GPS location must be newer than this to be considered valid

// The TinyGPS++ object
TinyGPSPlus gps;
const int UTC_offset = -7; 
unsigned long lastSerial = 0;
unsigned long lastPublish = 0;
unsigned long startFix = 0;
bool gettingFix = false;

float lat,lon,alt;
float lat_ref, lon_ref;

void setup()
{
	Serial.begin(9600);

	// The GPS module initialization
	Serial1.begin(9600);
    startFix = millis();
    gettingFix = true;
	
	// OLED Display initialization
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	helloWorld();

	pinMode(D2,INPUT_PULLDOWN);
}

void loop()
{
	while (Serial1.available() > 0) {
		if (gps.encode(Serial1.read())) {
			displayInfo();
		}
	}
}

void displayInfo() {
	float lat,lon,alt;
	uint8_t hr,mn,se,cs,sat;
	if (millis() - lastSerial >= SERIAL_PERIOD) {
		lastSerial = millis();

		char buf[128];
		if (gps.location.isValid() && gps.location.age() < MAX_GPS_AGE_MS) {
			lat = gps.location.lat();
			lon = gps.location.lng(); 
			alt = gps.altitude.meters();
			hr = gps.time.hour();
			mn = gps.time.minute();
			se = gps.time.second();
			cs = gps.time.centisecond();
			sat = gps.satellites.value();

			if(hr > 7) {
				hr = hr + UTC_offset;
			}
			else {
				hr = hr + 24 + UTC_offset;
			}
			Serial.printf("Time: %02i:%02i:%02i:%02i --- ",hr,mn,se,cs);
			Serial.printf("lat: %f, long: %f, alt: %f \n", lat,lon,alt);
			if (gettingFix) {
				gettingFix = false;
				unsigned long elapsed = millis() - startFix;
				Serial.printlnf("%lu milliseconds to get GPS fix", elapsed);
			}
			display.clearDisplay();
			display.setCursor(0,0);
			display.printf("Time: %02i:%02i:%02i \n",hr,mn,se);
			display.printf("lat    %f \nlong %f \nalt %f\n", lat,lon,alt);
			display.printf("satelites %i \n", sat);
			display.display();

			if(digitalRead(D2)) {
				sendJSON(lat,lon,alt);
			}
		}
		else {
			strcpy(buf, "no location");
			if (!gettingFix) {
				gettingFix = true;
				startFix = millis();
			}
		}
	}
}

void helloWorld() {
	display.clearDisplay();
	display.setTextSize(2);
  	display.setTextColor(WHITE);
  	display.setCursor(20,5);
  	display.println("  GPS\n Starting");
	display.display();
	display.setTextSize(1);
}

void sendJSON(float jlat, float jlon, float jalt) {
	JsonWriterStatic<256> jw;
	int vibration;

	vibration = random(1,11);

    JsonWriterAutoObject obj(&jw);

    jw.insertKeyValue("Latitude", jlat);
    jw.insertKeyValue("Longitude", jlon);
    jw.insertKeyValue("Altitude", jalt);
    jw.insertKeyValue("Vibration", vibration);
  
	Particle.publish("gps_data",jw.getBuffer(), PRIVATE);

	display.clearDisplay();
	display.setTextSize(2);
  	display.setTextColor(WHITE);
  	display.setCursor(20,5);
  	display.println("Publish");
	display.display();
	display.setTextSize(1);

}