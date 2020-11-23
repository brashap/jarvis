/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/IoTPa/Documents/jarvis/FunkyFunk2/src/FunkyFunk2.ino"
/*
 * Project FunkyFunk2
 * Description: For Saige - method two of making an "array" of functions
 * Author: Brian Rashap
 * Date: 22-Nov-2020
 */

//Declare array of funcitons
void setup();
void loop();
int add(int x,int y);
int sub(int x,int y);
int mult(int x,int y);
int divi(int x,int y);
#line 9 "c:/Users/IoTPa/Documents/jarvis/FunkyFunk2/src/FunkyFunk2.ino"
int (* funky[4])(int x, int y) = {add,sub,mult,divi};

//Declare global variables
int a,b;
int answer;
int i;

void setup() {
  Serial.begin(9600);
}

void loop() {
  a = random(0,100);
  b = random(0,100);
  for(i=0;i<4;i++) {
    answer = funky[i](a,b);
    Serial.printf("For function %i: a = %i and b = %i equals %i \n",i,a,b,answer);  
    delay(250);
  }
  Serial.printf("\n\n\n");
  delay(3000);
}

int add(int x,int y) {
  return x+y;
}

int sub(int x,int y) {
  return x-y;
}

int mult(int x,int y) {
  return x*y;
}

int divi(int x,int y) {
  return x/y;
}