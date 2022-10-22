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
limit PauseSwitch = limit(Brain.ThreeWirePort.E);


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


//variables
float speed = 0;
double rightTemp = 0;
double rightTemp2 = 0;
double leftTemp = 0;
double leftTemp2 = 0;
int torque = 100;
int vel = 100;
float rightSprocketVel = 0;
float leftSprocketVel = 0;

//motor setup
void motorSetup(){
  RightMotor.setVelocity(vel, percent);
  RightMotor.setMaxTorque(torque, percent);

  RightMotor2.setVelocity(vel, percent);
  RightMotor2.setMaxTorque(torque, percent);

  LeftMotor.setVelocity(vel, percent);
  LeftMotor.setMaxTorque(torque, percent);

  LeftMotor2.setVelocity(vel, percent);
  LeftMotor2.setMaxTorque(torque, percent);

  BallMotor.setVelocity(100, percent);
  BallMotor.setMaxTorque(100, percent);
}

void motorStart(){
  RightMotor.spin(forward);
  RightMotor2.spin(forward);
  wait(2, seconds);
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
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("Right Motor Temperature: %f", rightTemp);
  Brain.Screen.newLine();
  Brain.Screen.print("Right Motor 2 Temperature: %f", rightTemp2);
  Brain.Screen.newLine();
  Brain.Screen.print("Left Motor Temperature: %f", leftTemp);
  Brain.Screen.newLine();
  Brain.Screen.print("Left Motor 2 Temperature: %f", leftTemp2);
  wait(0.05, seconds);
}

void launch(){
  BallMotor.spinFor(reverse, 180, degrees);
  wait(1, seconds);
  BallMotor.spinFor(forward, 180, degrees);
}

void encoderStuff(){
  //velocity of the sprockets
  rightSprocketVel = RightEncoder.velocity(rpm) * 2 * 3.14; //in radians/s
  leftSprocketVel = LeftEncoder.velocity(rpm) * 2 * 3.14; //in radians/s

  double sprocketInDiameter = 0.0762; //m
  double sprocketOutDiameter = 0.0381; //m
  double wheelDiameter = 0.06985; //m
  double sprocketOutMass = 0.009; //kg
  double wheelMass = 0.05; //kg

  double rightWheelVel = ((sprocketOutMass * (rightSprocketVel*sprocketInDiameter / sprocketOutDiameter) * (sprocketOutDiameter * sprocketOutDiameter)) / (wheelMass * (wheelDiameter * wheelDiameter)));
  double leftWheelVel = ((sprocketOutMass * (leftSprocketVel*sprocketInDiameter / sprocketOutDiameter) * (sprocketOutDiameter * sprocketOutDiameter)) / (wheelMass * (wheelDiameter * wheelDiameter)));

  //one of the wheels does not report speeds
  //this was typed on my mbp at 5:10pm
  //this was typed on my chromebook at 5:12pm

  //display speeds
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("Right Wheel Speed: %f", rightWheelVel);
  Brain.Screen.newLine();
  Brain.Screen.print("Left Whgeel Speed: %f", leftWheelVel);
  wait(0.05, seconds);

}

//stop all the motors
void stopAll(){
  RightMotor.stop();
  RightMotor2.stop();
  LeftMotor.stop();
  LeftMotor2.stop();
}

int main() {
  //motor setup
  motorSetup();
  
  while(true){
    //start if button is pressed
    if(Push.pressing()){
      motorStart(); //start motors
      wait(2, seconds);
      encoderStuff(); //record and display wheel speeds
      launch(); //shoot the marble
      wait(1, seconds);
      stopAll(); //stop the motors
    }
  }
}
