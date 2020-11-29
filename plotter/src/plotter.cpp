/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/IoTPa/Documents/jarvis/plotter/src/plotter.ino"
/*
 * Project plotter
 * Description:
 * Author:
 * Date:
 */
#include "math.h"

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

void setup();
void loop();
void oledPlot(int y);
int mapFloat(float fromY, float fromMin, float fromMax, float toMin, float toMax);
#line 12 "c:/Users/IoTPa/Documents/jarvis/plotter/src/plotter.ino"
#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);

const int yMax = 100;
const int yMin = 0;

const float freq = .1;
const int amp = 40;
const int offset = 50;
const unsigned int plotPeriod = 5; //x-axis steps in ms
float data;
unsigned int currentTime;
unsigned int lastTime;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.setRotation(2);
  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer
}


void loop() {
  currentTime = millis();
  if((currentTime-lastTime)>plotPeriod) {
    data = amp*sin(2*M_PI*freq*(currentTime/1000.0))+offset;
    oledPlot(data);
    lastTime = currentTime;
    Serial.printf("Time = %f, Data = %i \n",currentTime/1000.0,data);
  }
}

void oledPlot(int y) {
  static int x=0;

  y = map(y,yMin,yMax,63,0);
  display.drawPixel(x, y, WHITE);
  display.display();
  x++;
  if(x>127) {
    x=0;
    display.clearDisplay();
  }
}

int mapFloat(float fromY, float fromMin, float fromMax, float toMin, float toMax) {
  return (fromY - fromMin) * (toMax - toMin) / (fromMax - fromMin) + fromMin;
}