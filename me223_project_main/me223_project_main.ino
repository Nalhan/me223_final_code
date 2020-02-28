// me223_project_main.ino
// Code for driving the 'Kar'
// Section 2 Station 4
// Eggstraordinary Gentlemen

// include libraries to allow simultaneous driving of stepper motors
#include <AccelStepper.h>
#include <MultiStepper.h>

// NOTE: const ints save memory for things like defining pins
// Because they never need to be modified, the compiler replaces
// instances where they appear with the value itself
// similar to #define

// Stepper motor mode
const int HALFSTEP = 8;

// Define pins for stepper 1
const int in1 = 2;
const int in2 = 3;
const int in3 = 4;
const int in4 = 5;

// define pins for stepper 2
const int in1_2 = 6;
const int in2_2 = 7;
const int in3_2 = 8;
const int in4_2 = 9;

// define pins for DC motor

const int motorfwd = 10;
const int motorbkwd = 11;

int triggerLevel = 380; // light sensor trigger level


// Distance values for our steppers in steps
// These values are called to move to these coordinates
// Where the first value is the open/close motor
// The second value is the up/down motor
long clawMidpoint[2] = {-1600, -1600};
long clawLowpoint[2] = {-3400, -3400};
long clawMidOpen[2] = {-1200, -1800};
long clawDownOpen[2] = {-2400, -3000};
long clawUp[2] = {0, 0};

long claw5Mid[2] = {-500, -500};
long claw5MidOpen[2] = {-100, -700};
long claw5DownOpen[2] = {-300, -900};
long claw5DownClose[2] = {-1200, -1200};

long clawPit[2] = {-4096, -4096};
//long clawUpLate[2] = {-100,-100};

// initialize steppers; NOTE pin order 1,3,2,4

AccelStepper stepper1(HALFSTEP, in1, in3, in2, in4);
AccelStepper stepper2(HALFSTEP, in1_2,in3_2,in2_2,in4_2);

MultiStepper steppers;



void setup() {
  
  // open serial and set speed values for steppers
  Serial.begin(9600);
  stepper1.setMaxSpeed(1000);
  stepper2.setMaxSpeed(1000);
  stepper1.setAcceleration(1000);
  stepper2.setAcceleration(1000);
  stepper1.setSpeed(1000);
  stepper2.setSpeed(1000);

  // add steppers to multistepper class for synchronized control
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);

  // set pin mode for dc motor and light sensor
  
  pinMode(motorfwd, OUTPUT);
  pinMode(motorbkwd, OUTPUT);
  pinMode(A0, INPUT);


  // delay 25 sec, then drive back, then stop after 4 sec
  delay(25000);
  driveBackward();
  delay(4000);
  driveStop();
}

// variables that are flipped when conditions are met
bool wait1 = 1;
bool wait2 = 1;

void loop() {
  while(wait1 == 1) {
   int sensor = analogRead(A0);
    if(sensor > triggerLevel) {
      // Code to drive claw
      steppers.moveTo(clawMidpoint); // Lower claw to midpoint
      steppers.runSpeedToPosition();
      steppers.moveTo(clawMidOpen); // Open claw
      steppers.runSpeedToPosition();
      steppers.moveTo(clawDownOpen); // Lower open claw
      steppers.runSpeedToPosition();
      stepper1.setSpeed(1200);
      steppers.moveTo(clawLowpoint); // Raise to midpoint
      steppers.runSpeedToPosition();
      stepper1.setSpeed(1000);
      steppers.moveTo(clawUp);        // raise to top
      steppers.runSpeedToPosition();
      wait1 = 0;
    }
    delay(200);
  }

  driveForward();
  delay(2000); // wait 2 sec before doing claw stuff

  steppers.moveTo(clawPit);  // Lower claw down
  steppers.runSpeedToPosition();
  steppers.moveTo(clawUp);    // Lift claw up
  steppers.runSpeedToPosition();
  
  delay(1000); // delay a little longer to reach destination
  driveStop();
      
  steppers.moveTo(claw5Mid); // Lower claw to midpoint
  steppers.runSpeedToPosition();
  steppers.moveTo(claw5DownOpen); // Lower more and open
  steppers.runSpeedToPosition();
  steppers.moveTo(clawUp);      // bring claw up
  steppers.runSpeedToPosition();
  
  // move backward out of the way for station 5
  driveBackward();
  delay(2500);
  driveStop();
  
  while(wait2 == 1) {
    int sensor = analogRead(A0); // Read in value from light sensor every iteration
    // if sensor detects light above the triggerLevel
    if(sensor > triggerLevel) { 
      // get back in position
      driveForward();
      delay(2500);
      driveStop();
      
      steppers.moveTo(claw5Mid); // Lower claw to midpoint
      steppers.runSpeedToPosition();
      steppers.moveTo(claw5MidOpen); // Open claw
      steppers.runSpeedToPosition();
      steppers.moveTo(claw5DownOpen); // Lower open claw
      steppers.runSpeedToPosition();
      stepper1.setSpeed(1200);
      steppers.moveTo(claw5DownClose); // Raise to midpoint
      steppers.runSpeedToPosition();
      stepper1.setSpeed(1000);
      steppers.moveTo(clawUp);        // raise to top
      steppers.runSpeedToPosition();
      wait2 = 0;
    }
    delay(200);
  }

  // begin driving back
  driveBackward();
  delay(1000);
  
  steppers.moveTo(clawPit);  // Lower claw down
  steppers.runSpeedToPosition();
  steppers.moveTo(clawUp);    // Lift claw up
  steppers.runSpeedToPosition();

  delay(2000); // delay long enough to make it back to 3
  driveStop();

  steppers.moveTo(clawMidpoint); // Lower claw to midpoint
  steppers.runSpeedToPosition();
  steppers.moveTo(clawDownOpen); // Lower more and open
  steppers.runSpeedToPosition();
  steppers.moveTo(clawUp);      // bring claw up
  steppers.runSpeedToPosition();
  
  // get out of the way of station 3 then stop
  driveForward();
  delay(3000);
  driveStop();

  // infinite loop to keep car from doing anything else
  while(true) {
  }
  
}

//void driveForward() Call to drive DC motor forward
void driveForward() {
  digitalWrite(motorfwd, HIGH);
  digitalWrite(motorbkwd, LOW);  
}

//void driveBackward() Call to drive DC motor backward
void driveBackward() {
  digitalWrite(motorfwd, LOW);
  digitalWrite(motorbkwd, HIGH);
} 

//void driveStop() Call to stop DC motor
void driveStop() {
  digitalWrite(motorfwd, LOW);
  digitalWrite(motorbkwd, LOW);
}
