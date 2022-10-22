#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
motor RightMotor = motor(PORT2, ratio36_1, false);

motor LeftMotor = motor(PORT5, ratio36_1, true);

motor BallMotor = motor(PORT3, ratio36_1, false);

bumper Push = bumper(Brain.ThreeWirePort.H);
motor RightMotor2 = motor(PORT1, ratio36_1, true);

motor LeftMotor2 = motor(PORT4, ratio36_1, false);

encoder LeftEncoder = encoder(Brain.ThreeWirePort.A);
encoder RightEncoder = encoder(Brain.ThreeWirePort.C);
limit ArmingSwitch = limit(Brain.ThreeWirePort.E);
light BallLight = light(Brain.ThreeWirePort.G);
led ArmedLEDs = led(Brain.ThreeWirePort.F);


#pragma endregion VEXcode Generated Robot Configuration

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Nicholas D. Thoennes                                      */
/*    Created:      2/16/22                                                   */
/*    Description:  Class Marble Maze                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// Include the V5 Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;


//todo
/*
  - adjust motor speed so they're the same
  - potentiometer variable speed
  - 
*/

//variables
float speed = 0;
double rightTemp = 0;
double rightTemp2 = 0;
double leftTemp = 0;
double leftTemp2 = 0;
int torque = 100;
int vel = 75;
int rightVel = 85;
int leftVel = 83;
float rightSprocketVel = 0;
float leftSprocketVel = 0;

//motor setup
void motorSetup(){
  
  //right motors
  RightMotor.setVelocity(85, rpm);
  RightMotor.setMaxTorque(torque, percent);
  RightMotor2.setVelocity(85, rpm);
  RightMotor2.setMaxTorque(torque, percent);

  //left motors
  LeftMotor.setVelocity(83, rpm);
  LeftMotor.setMaxTorque(torque, percent);
  LeftMotor2.setVelocity(83, rpm);
  LeftMotor2.setMaxTorque(torque, percent);

  //ball motor
  BallMotor.setVelocity(100, percent);
  BallMotor.setMaxTorque(100, percent);
}

void motorStart(){
  RightMotor.spin(forward);
  RightMotor2.spin(forward);
  LeftMotor.spin(forward);
  LeftMotor2.spin(forward);
}

void motorTemps(){
  //record temps
  rightTemp = RightMotor.temperature(percent);
  rightTemp2 = RightMotor2.temperature(percent);
  leftTemp = LeftMotor.temperature(percent);
  leftTemp2 = LeftMotor2.temperature(percent);
  
  //display temps
  // Brain.Screen.clearScreen();
  // Brain.Screen.setCursor(1, 1);
  // Brain.Screen.print("Right Motor Temperature: %f", rightTemp);
  // Brain.Screen.newLine();
  // Brain.Screen.print("Right Motor 2 Temperature: %f", rightTemp2);
  // Brain.Screen.newLine();
  // Brain.Screen.print("Left Motor Temperature: %f", leftTemp);
  // Brain.Screen.newLine();
  // Brain.Screen.print("Left Motor 2 Temperature: %f", leftTemp2);
  // wait(0.05, seconds);
}

//pushes the ball carriiage forwards some, then back
void launch(){
  BallMotor.spinFor(reverse, 180, degrees);
  wait(1, seconds);
  BallMotor.spinFor(forward, 180, degrees);
}

void encoderStuff(double* rightVel, double* leftVel){ 

  //record sprocket shaft speeds
  rightSprocketVel = RightEncoder.velocity(rpm); //in rpm
  leftSprocketVel = LeftEncoder.velocity(rpm); //in rpm

  // m/s calculations
  rightSprocketVel = RightEncoder.velocity(rpm) * 2 * 3.14; //in radians/s
  leftSprocketVel = LeftEncoder.velocity(rpm) * 2 * 3.14; //in radians/s

  //constants
  double sprocketInDiameter = 0.0762; //m
  double sprocketOutDiameter = 0.0381; //m
  double wheelDiameter = 0.06985; //m
  double sprocketOutMass = 0.009; //kg
  double wheelMass = 0.05; //kg

  double rightWheelVel = ((sprocketOutMass * (rightSprocketVel*sprocketInDiameter / sprocketOutDiameter) * (sprocketOutDiameter * sprocketOutDiameter)) / (wheelMass * (wheelDiameter * wheelDiameter)));
  double leftWheelVel = ((sprocketOutMass * (leftSprocketVel*sprocketInDiameter / sprocketOutDiameter) * (sprocketOutDiameter * sprocketOutDiameter)) / (wheelMass * (wheelDiameter * wheelDiameter)));

  *rightVel = RightEncoder.velocity(rpm);
  *leftVel = LeftEncoder.velocity(rpm);


  //display speeds
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  //Brain.Screen.print("Right Wheel Speed: %f rpm", (rightWheelVel)); //for rpm
  Brain.Screen.print("Right Wheel Speed: %f m/s", (rightWheelVel * wheelDiameter)); //for m/s
  Brain.Screen.newLine();
  //Brain.Screen.print("Left Wheel Speed: %f rpm", (leftWheelVel)); //for rpm
  Brain.Screen.print("Left Whgeel Speed: %f m/s", (leftWheelVel * wheelDiameter)); //for m/s
  wait(0.05, seconds);
}

void motorSpeeds(){
  //display speeds
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("Right Motor Speed: %f rpm", (RightMotor.velocity(rpm))); //for rpm
  Brain.Screen.newLine();
  Brain.Screen.print("Left Motor Speed: %f rpm", (LeftMotor.velocity(rpm))); //for rpm
  wait(0.05, seconds);
}

//stop all the motors
void stopAll(){
  RightMotor.stop();
  RightMotor2.stop();
  LeftMotor.stop();
  LeftMotor2.stop();
}

//manual firing
void manualFiring(){
  //if the bumper is pressed, fire
  if (Push.pressing()){
    motorStart(); //start motors
    wait(3, seconds);
    motorSpeeds(); //record and display motor speeds
    //encoderStuff(); //record and display wheel speeds
    launch(); //shoot the marble
    wait(1, seconds);
    stopAll(); //stop the motors
  }
}

//autonomous
void autonomousFiring(){
  if (BallLight.brightness() < 55){
    motorStart(); //start motors
    wait(3, seconds);
    motorSpeeds(); //record and display motor speeds
    //encoderStuff(); //record and display wheel speeds
    launch(); //shoot the marble
    wait(1, seconds);
    stopAll(); //stop the motors
  }
}


/*this doesn't work. while you cen tell the motors to spin at the same velocity, in reality they will not.
this was going to take the varying speeds from the encoders which are on the 12T gear 2 shaft and
work backwards to find the velocities of the motors using simple machines gear ratio calculations. using a loop (with tolerance)
I would adjust the faster motor until it comes close to the slower motor's speed. ran outta time tho :(*/

//convert velocities coming from the encoders to find the velocities of the motors
void convert(double* rightVel, double* leftVel){
  //gear ratio calculations
  //motor is 36:1
  //gear 1 is 84T
  //gear 2 is 12T
  //sprocket diameter is ?
}

//the motors don't spin at exactly the same speed in reality
//offset them so they're at least very close
void speedOffsets(){
  double rightVel = 0.1;
  double leftVel = 0.2;
  while(rightVel != leftVel){ //while the two motor speeds aren't equal
    encoderStuff(&rightVel, &leftVel); //update the motor speed variables
    if(rightVel > leftVel){ //if right is spinning faster than left
      rightVel = rightVel - 0.5; //decrement rightVel by 0.5 rpm until it is no longer faster than left

     //convert rightVel because gears
    convert(&rightVel, &leftVel);

      RightMotor.setVelocity(rightVel, rpm); //set the motor speeds
      RightMotor2.setVelocity(rightVel, rpm);
    }
    else if(leftVel > rightVel){ //otherwise if left is spinning faster than right
      leftVel = leftVel - 0.5; //decrement leftVel by 0.5 rpm until it is no longer faster than right

      //convert leftVel because gears

      LeftMotor.setVelocity(leftVel, rpm); //set the motor speeds
      LeftMotor2.setVelocity(leftVel, rpm);
    }
  }
}

int main() {
  //motor setup
  motorSetup();
  ArmedLEDs.off();

/*start a main loop
-if the arming switch is pressed
  -turn on the LEDs
  -start the autonomous loop
  -if the arming switch is pressed
    - turn off the LEDs
    - break the autonomous loop
  -if it is not being pressed
    -call autonomous firing
-if it is not being pressed
  -call manual firing
*/
  while(true){
    if(ArmingSwitch.pressing()){
      ArmedLEDs.on();
      while(true){
        if(ArmingSwitch.pressing()){ //disarm
          ArmedLEDs.off();
          break; //break the autonomous loop
        }
        else{ //otherwise fire autonomously
          autonomousFiring();
        }
      }
    }
    else{ //otherwise fire manually
      manualFiring();
    }
  }
}