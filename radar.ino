
#include <AccelStepper.h>           // Load the AccelStepper library
#include <NewPing.h>

#define motorPin1  2                // IN1 pin on the ULN2003A driver
#define motorPin2  3                // IN2 pin on the ULN2003A driver
#define motorPin3  4               // IN3 pin on the ULN2003A driver
#define motorPin4  5               // IN4 pin on the ULN2003A driver
#define TRIG_PIN 10
#define ECHO_PIN 11
#define MAX_DISTANCE 400  // Maximum distance (in cm) to measure
int stepsPerRevolution = 64;        // steps per revolution
float degreePerRevolution = 5.625;  // degree per revolution

/*
 * AccelStepper::FULL2WIRE (2) means: 2 wire stepper (2 pins needed). 
 * AccelStepper::FULL3WIRE (3) means: 3 wire stepper, like a harddisk motor (3 pins needed). 
 * AccelStepper::FULL4WIRE (4) means: 4 wire stepper (4 pins needed). 
 * AccelStepper::HALF3WIRE (6) means: 3 wire half stepper, like a harddisk motor (3 pins needed) 
 * AccelStepper::HALF4WIRE (8) means: 4 wire half stepper (4 pins needed) 
 *
 * AccelStepper uses AccelStepper::FULL4WIRE (4 pins needed) by default.
 */
AccelStepper stepperRound(AccelStepper::HALF4WIRE, motorPin1, motorPin3, motorPin2, motorPin4);
AccelStepper stepperVertical(AccelStepper::HALF4WIRE, motorPin1+4, motorPin3+4, motorPin2+4, motorPin4+4);

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);               // initialise the serial monitor

  stepper.setMaxSpeed(1000.0);      // set the max motor speed
  stepper.setAcceleration(100.0);   // set the acceleration
  stepper.setSpeed(200);            // set the current speed

}

void loop() {
  float degRound = 0;
  for(int i=0;i<32;++i){
    stepperRound.moveTo(degToSteps(degRound));
    while(stepperRound.distanceToGo()){
        stepperRound.run();
    }
    float degVert = 0;
    for(int j=0;j<32;++j){
      stepperVertical.moveTo(degToSteps(degVert));
      while(stepperVertical.distanceToGo()){
        stepperVertical.run();
      }
      unsigned int distance = sonar.ping_cm();
      if(distance == 0){
        distance = MAX_DISTANCE;
      }
      Serial.write(i);
      Serial.write(",");
      Serial.write(j);
      Serial.write(",");
      Serial.write(distance);
      Serial.write("\n");
      degVert+=degreePerRevolution;
    }
    stepperVertical.moveTo(degToSteps(-180));//restore position
    degRound+=degreePerRevolution;
  }
  stepperRound.moveTo(degToSteps(-180));//restore position
}

/*
 * Converts degrees to steps
 * 
 * 28BYJ-48 motor has 5.625 degrees per step
 * 360 degrees / 5.625 = 64 steps per revolution
 * 
 * Example with degToSteps(45):
 * (64 / 5.625) * 45 = 512 steps
 */
float degToSteps(float deg) {
  return (stepsPerRevolution / degreePerRevolution) * deg;
}