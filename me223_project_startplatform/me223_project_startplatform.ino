// me223_project_startplatform.ino
// Code for driving servos on the start platform
// Section 2 Station 4
// Eggstraordinary Gentlemen

// include servo library
#include <Servo.h>

// initalize servo objects
Servo servo1;
Servo servo2;

// position variables for servos 1 and 2
int pos1 = 0;
int pos2 = 0;
void setup() {
  servo1.attach(2); // attach servo on pin 2 to object servo1
  servo1.write(80);	// set servo1 to 80 degrees (this angle had the forks straight up)
  delay(250);
  servo2.attach(3); // attach servo on pin 3 to object servo2
  servo2.write(80); // set servo1 to 80 degrees (this angle had the forks straight up)
  delay(150);

  pos2 = 80;
  // loop through to move servos 50 degrees to bump one side of tracks
  for(pos1 = 80; pos1 >= 30; pos1--){
      pos2++;
      servo1.write(pos1);
      servo2.write(pos2);
      delay(5);
    }
  

  delay(1000);
  // loop through to move servos 100 degrees in other direction to bump other side of tracks
  for(pos1 = 30; pos1 <= 150; pos1++) {
      pos2--;
      servo1.write(pos1);
      servo2.write(pos2);
      delay(5);
    }
  
  delay(150);
  
  // bring servos back to 80 degrees
  servo1.write(80);
  delay(10);
  servo2.write(80);
}

void loop() {
	// unused because we only need to do the above once
}
