/*
* Project Time_Sync
* Description : Function to Sync Time to Particle Cloud
* Author : Brian Rashap
* Date : 13-Jan -2020
*/

# include <Particle.h>

void setup () {
  Serial.begin();
  while (!Serial); // wait for Serial monitor
  Serial.println (" -------- Begin Time Sync --------");
}

void loop () {
  sync_my_time ();
  delay(random(10000,60000));
}

void sync_my_time() {
  String DateTime;
  String TimeOnly;
  char currentDateTime[25];
  char currentTime[9];

  Time.zone(-6); // Set Time Zone to MDT (UTC - 7)
  unsigned long cur=millis();

  // Request time synch from Particle Device Cloud and wait for data to come back
  Particle.syncTime();
  waitUntil(Particle.syncTimeDone);

  // Check if synchronized successfully
  if(Particle.timeSyncedLast() >= cur) {
    DateTime = Time.timeStr();
    TimeOnly = DateTime.substring(11,19);       // pull out only Time from DateTime String
    Serial.println(DateTime);
    Serial.println(TimeOnly);

    // Using Time with Formatted Print Statements

    DateTime.toCharArray(currentDateTime,25);   // convert DateTime in to an array of char for printf()
    TimeOnly.toCharArray(currentTime,9);        // convert TImeOnly to an array of char for printf()
    Serial.printf("The current date and time is %s \n",currentDateTime);
    Serial.printf("The current time is %s \n",currentTime);
  }
}