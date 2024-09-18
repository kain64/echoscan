
#include <AccelStepper.h>           // Load the AccelStepper library
#include <NewPing.h>

#define motorPin1  2                // IN1 pin on the ULN2003A driver
#define motorPin2  3                // IN2 pin on the ULN2003A driver
#define motorPin3  4               // IN3 pin on the ULN2003A driver
#define motorPin4  5               // IN4 pin on the ULN2003A driver

#define motor2Pin1  6                // IN1 pin on the ULN2003A driver
#define motor2Pin2  7                // IN2 pin on the ULN2003A driver
#define motor2Pin3  8               // IN3 pin on the ULN2003A driver
#define motor2Pin4  9               // IN4 pin on the ULN2003A driver

#define TRIG_PIN 10
#define ECHO_PIN 11
#define MAX_DISTANCE 400  // Maximum distance (in cm) to measure
int stepsPerRevolution = 64;        // steps per revolution
float degreePerRevolution = 5.625;  // degree per revolution
#define INIT_ANGLE 0
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
AccelStepper stepperVertical(AccelStepper::HALF4WIRE, motor2Pin1, motor2Pin3, motor2Pin2, motor2Pin4);

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(9600);               // initialise the serial monitor

  stepperRound.setMaxSpeed(1000.0);      // set the max motor speed
  stepperRound.setAcceleration(100.0);   // set the acceleration
  stepperRound.setSpeed(200);            // set the current speed
  stepperVertical.setMaxSpeed(1000.0);      // set the max motor speed
  stepperVertical.setAcceleration(100.0);   // set the acceleration
  stepperVertical.setSpeed(200);            // set the current speed

  // stepperVertical.moveTo(degToSteps(-45));//restore position
  // while(stepperVertical.distanceToGo()){
  //     stepperVertical.run();
  // }
  // stepperRound.moveTo(degToSteps(-45));//restore position
  // while(stepperRound.distanceToGo()){
  //     stepperRound.run();
  // }

}

void loop() {
  float degRound = INIT_ANGLE;
  Serial.println("START !!!");
  for(int i=0;i<16;++i){
    stepperRound.moveTo(degToSteps(degRound));
    while(stepperRound.distanceToGo()){
        stepperRound.run();
    }
    float degVert = INIT_ANGLE;
    for(int j=0;j<16;++j){
      stepperVertical.moveTo(degToSteps(degVert));
      while(stepperVertical.distanceToGo()){
        stepperVertical.run();
      }
      delay(50);
     
      unsigned int uS = sonar.ping();  // Get time in microseconds
      float distance = (uS / 2.0) * 0.0343;  // Convert to distance in cm (Speed of sound: 343 m/s)
      Serial.print(i);
      Serial.print(",");
      Serial.print(j);
      Serial.print(",");
      Serial.print(distance * 10);
      Serial.println();
      degVert+=degreePerRevolution;
    }
    stepperVertical.moveTo(degToSteps(INIT_ANGLE));//restore position
    while(stepperVertical.distanceToGo()){
        stepperVertical.run();
    }
    degRound+=degreePerRevolution;
  }
  
  stepperRound.moveTo(degToSteps(INIT_ANGLE));//restore position
  while(stepperRound.distanceToGo()){
        stepperRound.run();
  }
  Serial.println("finish !!!");
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