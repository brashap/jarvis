/*
 * Project FunkyFunk
 * Description: D. Brian's method ONE of making an "array" of functions
 * Author: Brian Rashap
 * Date: 22-Nov-2020
 */

int x,y;
int answer;
int i;

// setup() runs once, when the device is first turned on.
void setup() {
  Serial.begin(9600);
}


void loop() {
  x = random(0,100);
  y = random(0,100);
  for(i=0;i<4;i++) {
    answer = funky(i,x,y);
    Serial.printf("For function %i: x = %i and y = %i equals %i \n",i,x,y,answer);  
    delay(250);
  }
  Serial.printf("\n\n\n");
  delay(3000);
}

int funky(int whichFunk, int a, int b) {
  int result;

  switch(whichFunk) {
    case 0: 
      result = add(a,b);
      break;
    case 1:
      result = sub(a,b);
      break;
    case 2:
      result = mult(a,b);
      break;
    case 3:
      result = divi(a,b);
      break;
    default:
      result = 42;
  }
  return result;
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