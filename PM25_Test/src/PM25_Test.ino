/*
 * Project PM25
 * Description: 2.5um Particle Measurement with H3301 Sensor
 * Author:  Brian Rashap
 * Date:    17-APR-2020
 */
//#include <Particle.h>
#include <Seeed_HM330X.h>
#include <Wire.h>

/****************SetUp HM330X**************/
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

HM330XErrorCode print_result(const char* str, uint16_t value) {
    if (NULL == str) {
        return ERROR_PARAM;
    }
    Serial.print(str);
    Serial.println(value);
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
        Serial.print(data[i], HEX);
        Serial.print("  ");
        if ((0 == (i) % 5) || (0 == i)) {
            Serial.println("");
        }
    }
    uint8_t sum = 0;
    for (int i = 0; i < 28; i++) {
        sum += data[i];
    }
    if (sum != data[28]) {
        Serial.println("wrong checkSum!!!!");
    }
    Serial.println("");
    return NO_ERROR1;
}

/************Declare Variables*************/
int status;

void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println("----------------------------------------------------------");

  Serial.println("Serial start");
  if (sensor.init()) {
    Serial.println("HM330X init failed!!!");
    while (1);
    }
}

void loop() {

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
  delay(5000);
}
