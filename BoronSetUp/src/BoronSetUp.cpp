/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "c:/Users/IoTPa/Documents/jarvis/BoronSetUp/src/BoronSetUp.ino"
/*
 * Project BoronSetUp
 * Description: Setup external SIM on boron for T-Mobile
 * Author: Brian Rashap
 * Date: 19-Nov-2020
 */

#include "Particle.h"

#include "dct.h"

void setup();
void loop();
#line 12 "c:/Users/IoTPa/Documents/jarvis/BoronSetUp/src/BoronSetUp.ino"
SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
	Cellular.setActiveSim(EXTERNAL_SIM);
	Cellular.setCredentials("fast.t-mobile.com");

	// This clears the setup done flag on brand new devices so it won't stay in listening mode
	const uint8_t val = 0x01;
    dct_write_app_data(&val, DCT_SETUP_DONE_OFFSET, 1);

	// This is just so you know the operation is complete
	pinMode(D7, OUTPUT);
	digitalWrite(D7, HIGH);
}

void loop() {
}