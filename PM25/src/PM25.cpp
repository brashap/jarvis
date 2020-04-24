/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/IoTPa/Documents/jarvis/PM25/src/PM25.ino"
/*
 * Project PM25
 * Description: 2.5um Particle Measurement with H3301 Sensor
 * Author:  Brian Rashap
 * Date:    17-APR-2020
 */
//#include <Particle.h>
#include <Seeed_HM330X.h>
#include <JsonParserGeneratorRK.h>

/***************SetUp BME280*****************/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

HM330XErrorCode print_result(const char* str, uint16_t value);
HM330XErrorCode parse_result(uint8_t* data);
HM330XErrorCode parse_result_value(uint8_t* data);
void setup();
void loop();
void createEventPayLoad(float tempValue, float presValue, float humValue, float gasValue, int PM25Value);
void printValues();
void printData();
void printhello();
#line 18 "c:/Users/IoTPa/Documents/jarvis/PM25/src/PM25.ino"
#define SEALEVELPRESSURE_HPA 1013.25
Adafruit_BME680 bme; // I2C

/****************SetUp HM330X**************/

#ifdef  ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL_OUTPUT SerialUSB
#else
    #define SERIAL_OUTPUT Serial
#endif

HM330X sensor;
uint8_t buf[30];
int PM25;

const char* str[] = {"sensor num: ", "PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                     "PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                     "PM10 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                     "PM1.0 concentration(Atmospheric environment,unit:ug/m3): ",
                     "PM2.5 concentration(Atmospheric environment,unit:ug/m3): ",
                     "PM10 concentration(Atmospheric environment,unit:ug/m3): ",
                    };

// Define OLED Display
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);  // the -1 means there is no reset pin
#define SSD1306_128_64     // 128x64 pixel display


HM330XErrorCode print_result(const char* str, uint16_t value) {
    if (NULL == str) {
        return ERROR_PARAM;
    }
    SERIAL_OUTPUT.print(str);
    SERIAL_OUTPUT.println(value);
    return NO_ERROR1;
}

/*parse buf with 29 uint8_t-data*/
HM330XErrorCode parse_result(uint8_t* data) {
    uint16_t value = 0;
    if (NULL == data) {
        return ERROR_PARAM;
    }
    for (int i = 1; i < 8; i++) {
        value = (uint16_t) data[i * 2] << 8 | data[i * 2 + 1];
        print_result(str[i - 1], value);

    }

    return NO_ERROR1;
}

HM330XErrorCode parse_result_value(uint8_t* data) {
    if (NULL == data) {
        return ERROR_PARAM;
    }
    for (int i = 0; i < 28; i++) {
        SERIAL_OUTPUT.print(data[i], HEX);
        SERIAL_OUTPUT.print("  ");
        if ((0 == (i) % 5) || (0 == i)) {
            SERIAL_OUTPUT.println("");
        }
    }
    uint8_t sum = 0;
    for (int i = 0; i < 28; i++) {
        sum += data[i];
    }
    if (sum != data[28]) {
        SERIAL_OUTPUT.println("wrong checkSum!!!!");
    }
    SERIAL_OUTPUT.println("");
    return NO_ERROR1;
}

/************Declare Variables*************/
double temp;
double pres;
double hum;
double alt;
double gas;
unsigned int status;

int delayStart;

void setup() {
  Serial.begin(9600);
  while(!Serial);

// Initialize BME280
    Serial.println(F("BME280 test"));

    SERIAL_OUTPUT.println("Serial start");
    if (sensor.init()) {
        SERIAL_OUTPUT.println("HM330X init failed!!!");
        while (1);
    }
    
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

    // initialize and clear display
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.display();

  printhello();
}

void loop() {
temp = (bme.readTemperature()*(9.0/5.0))+32;
pres = (bme.readPressure() / 100.0F * 0.02953)+5;
hum = bme.readHumidity();
gas = bme.readGas() / 1000.0;
printValues();

if (sensor.read_sensor_value(buf, 29)) {
    Serial.println("HM330X read result failed!!!");
    }
Serial.println("Starting Display");
parse_result_value(buf);
Serial.println("Printing Concentrations");
parse_result(buf);
Serial.println("PM2.5 Data");
PM25 = (uint16_t) buf[6 * 2] << 8 | buf[6 * 2 + 1];
print_result(str[6 - 1], PM25);
Serial.println("");

  
createEventPayLoad(temp,pres,hum,gas,PM25);
printData();
delay(60000);    
}

void createEventPayLoad(float tempValue, float presValue, float humValue, float gasValue, int PM25Value) {
  JsonWriterStatic<256> jw;
  {
    JsonWriterAutoObject obj(&jw);

    jw.insertKeyValue("Home_Temp", tempValue);
    jw.insertKeyValue("Home_Pres", presValue);
    jw.insertKeyValue("Home_Humid", humValue);
    jw.insertKeyValue("Home_Gas", gasValue);
    jw.insertKeyValue("Home_PM25", PM25Value);
  }
  Particle.publish("bme-vals",jw.getBuffer(), PRIVATE);
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

void printData() {

 

    display.clearDisplay();
    display.display();

     // display a pixel in each corner of the screen
    display.drawPixel(0, 0, WHITE);
    display.drawPixel(127, 0, WHITE);
    display.drawPixel(0, 63, WHITE);
    display.drawPixel(127, 63, WHITE);

    display.setCursor(0,5);
    display.printf("Temperature: %0.2f F\n",temp);
    display.printf("Pressure: %0.2f inHg\n",pres);
    display.printf("Humidity: %0.2f rH\n", hum);
    display.printf("Gas Conc: %0.2f\n",gas);
    display.printf("PM25: %i \n", PM25);

    display.display();
}

void printhello() {
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
  display.setCursor(27,30);
  display.print("Hello, world!");

  // update display with all of the above graphics
  display.display();
}