/*
 * Project PWM2ADC
 * Description: Code to test circuit to convert PWM to true ACD
 * Author: Brian Rashap
 * Date: 12-Nov-2021
 */

// Use with RC Filter - R = 220 ohms and C = 2.2uF

#include <math.h>


const int pwmPin = A0;
float t;
int freq = 50;
float vout;

void setup() {
  Serial.begin(9600);
  pinMode(pwmPin,OUTPUT);  
}

void loop() {
  t = millis()/1000.0;
  vout = 127*(sin(2*M_PI*freq*t)+1);
  Serial.printf("%0.2f\n",vout);
  analogWrite(A0,vout,5000);
}