// // ---- START VEXCODE CONFIGURED DEVICES ----
// // Robot Configuration:
// // [Name]               [Type]        [Port(s)]
// // ---- END VEXCODE CONFIGURED DEVICES ----
// /*----------------------------------------------------------------------------*/
// /*                                                                            */
// /*    Module:       main.cpp                                                  */
// /*    Author:       C:\Users\grega                                            */
// /*    Created:      Wed Oct 09 2019                                           */
// /*    Description:  V5 project                                                */
// /*                                                                            */
// /*----------------------------------------------------------------------------*/

#include "vex.h"
#include <unistd.h>
#include <stdio.h>

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
//brain Brain;

// controller instance
controller cntrlr = controller();

// motor instancnes
motor right_front = motor(PORT14, 0);
motor right_mid = motor(PORT15, 0);
motor right_back  = motor(PORT16, 0);
motor left_front = motor(PORT18, 1);
motor left_mid = motor(PORT19, 1);
motor left_back = motor(PORT20, 1);
// lift motor instances
motor lift_motor_0 = motor(PORT9, 1);
motor lift_motor_1 = motor(PORT10, 0);
// intake motor instances
motor intake_left = motor(PORT4, 0);
motor intake_right = motor(PORT5,1);
// hinge motor instances
motor hinge_left = motor(PORT11, 1);
motor hinge_right = motor(PORT12, 0);

// timer and cooldowns 
double intake_time = Brain.Timer.time(sec);
bool intake_engage = true;
bool hinge = true;

// hinge power (percent), tolerance (int), incrementer (degree)
double lift_pwr = .05;
double pwr = 3.5;
int tol = 1;

// initial potentiometer positions
double pot_left_pos = 72.8 + 18;
double pot_right_pos = 144.2 - 18;

// inch to wheel revolution conversion
const float gearRat = 1; // .5 turn of motor = 1 turn of wheel, 1 if directly connected to motor without gears    
const float wheel_dim = 4; //inches
const float wheel_cir  = wheel_dim * 3.1416; //dim * pi
const float turnDim = 21; //distance from top left wheel to bottom right wheel


void move_straight(float inches, float power) {
  const float inchesPerDegree =  wheel_cir / 360; 
  const float degrees = inches / inchesPerDegree;

  right_front.startRotateFor(degrees * gearRat, rotationUnits::deg, power, velocityUnits::pct);                  //concurrently start rotating all wheels
  right_mid.startRotateFor(degrees * gearRat, rotationUnits::deg, power, velocityUnits::pct);         
  right_back.startRotateFor(degrees * gearRat, rotationUnits::deg, power, velocityUnits::pct);
  left_front.startRotateFor(degrees * gearRat, rotationUnits::deg, power, velocityUnits::pct);
  left_mid.startRotateFor(degrees * gearRat, rotationUnits::deg, power, velocityUnits::pct);
  left_back.startRotateFor(degrees * gearRat, rotationUnits::deg, power, velocityUnits::pct);

  this_thread::sleep_for(50);
}

//Positive Degrees is a right turn
//Negative Degrees is a left turn
void turn(float degrees, float power) {
  const float turnRatio = turnDim / wheel_dim;
  const float wheelDeg = turnRatio * degrees;

  right_front.startRotateFor(wheelDeg * gearRat / 2, rotationUnits::deg, power, velocityUnits::pct);                //concurrently start rotating all wheels
  right_mid.startRotateFor(wheelDeg * gearRat / 2, rotationUnits::deg, power, velocityUnits::pct);     
  right_back.startRotateFor(wheelDeg * gearRat / 2, rotationUnits::deg, power, velocityUnits::pct); 
  left_front.startRotateFor(-wheelDeg * gearRat / 2, rotationUnits::deg, power, velocityUnits::pct);
  left_mid.startRotateFor(-wheelDeg * gearRat / 2, rotationUnits::deg, power, velocityUnits::pct);
  left_back.startRotateFor(-wheelDeg * gearRat / 2, rotationUnits::deg, power, velocityUnits::pct);

  this_thread::sleep_for(50);
}

void lift(float degrees, float power) {
  lift_motor_0.startRotateFor(degrees, rotationUnits::deg, power, velocityUnits::pct);
  lift_motor_1.startRotateFor(degrees, rotationUnits::deg, power, velocityUnits::pct);
  this_thread::sleep_for(5000);
  move_straight(1, 2.5);
  this_thread::sleep_for(1000);
  move_straight(-20, 5);
  this_thread::sleep_for(2000);
  lift_motor_0.startRotateTo(0, rotationUnits::deg, power, velocityUnits::pct);
  lift_motor_1.startRotateTo(0, rotationUnits::deg, power, velocityUnits::pct);
}

int hingePosition(){

      vex::pot pot_left = vex::pot(Brain.ThreeWirePort.A);
      vex::pot pot_right = vex::pot(Brain.ThreeWirePort.B);

      while (true) {

        intake_left.spin(fwd, 100, velocityUnits::pct);
        intake_right.spin(fwd, 100, velocityUnits::pct);

        if ((pot_left.value(deg) > pot_left_pos - tol) && (pot_left.value(deg) < pot_left_pos + tol)) { //if within tolerance, dont move 
          hinge_left.spin(fwd, 0, pct);
        } 
        else if (pot_left.value(deg) > pot_left_pos) { //move out
          hinge_left.spin(fwd, -pwr, pct);
        }
        else if (pot_left.value(deg) < pot_left_pos) //move in
        {
          hinge_left.spin(fwd, pwr, pct);
        }
        // right
        if ((pot_right.value(deg) < pot_right_pos + tol) && (pot_right.value(deg) > pot_right_pos - tol)) { // if within tolerance, dont move
          hinge_right.spin(fwd, 0, pct);
        } 
        else if (pot_right.value(deg) > pot_right_pos) { //move out
          hinge_right.spin(fwd, pwr, pct);
        }
        else if (pot_right.value(deg) < pot_right_pos) //move in
        {
          hinge_right.spin(fwd, -pwr, pct);
        }

        this_thread::sleep_for(50);
    }
  return 0;
}

int stack_tower() {

  vex::pot pot_left = vex::pot(Brain.ThreeWirePort.A);
  vex::pot pot_right = vex::pot(Brain.ThreeWirePort.B);

  double pot_left_poss = pot_left_pos - 22;
  double pot_right_poss  = pot_right_pos + 22;

  pwr = 3;

  while (true) {

    if ((pot_left.value(deg) > pot_left_poss - tol) && (pot_left.value(deg) < pot_left_poss + tol)) { //if within tolerance, dont move 
      hinge_left.spin(fwd, 0, pct);
    } 
    else if (pot_left.value(deg) > pot_left_poss) { //move out
      hinge_left.spin(fwd, -pwr, pct);
    }
    else if (pot_left.value(deg) < pot_left_poss) //move in
    {
      hinge_left.spin(fwd, pwr, pct);
    }
    // right
    if ((pot_right.value(deg) < pot_right_poss + tol) && (pot_right.value(deg) > pot_right_poss - tol)) { // if within tolerance, dont move
      hinge_right.spin(fwd, 0, pct);
    } 
    else if (pot_right.value(deg) > pot_right_poss) { //move out
      hinge_right.spin(fwd, pwr, pct);
    }
    else if (pot_right.value(deg) < pot_right_poss) //move in
    {
      hinge_right.spin(fwd, -pwr, pct);
    }
    
    this_thread::sleep_for(50);
  }
  return 0;
}

int main(){

  double second = 1000;
  float move_power = 8;
  double turn_ = 70;
  double turn_power = 15;

  // Set hinge position and intake to separate thread and start
  thread intake = thread(hingePosition);
  this_thread::sleep_for(second/2);

  // Drive Foward and intake 4 blocks
  move_straight(36, move_power);
  this_thread::sleep_for(second * 10);

  // Turn Right to approx 45 degrees
  turn(-turn_, turn_power);
  this_thread::sleep_for(second * 1.4);

  // Back up
  move_straight(-32.5, 30);
  this_thread::sleep_for(second * 3);

  // Turn Left to undo the previous turn
  turn((turn_+1), turn_power);
  this_thread::sleep_for(second * 1.5);

  // Drive Forward and intake other 5 blocks
  move_straight(34, move_power);
  this_thread::sleep_for(second * 8);

  // Turn Right to approx 90 degrees
  turn(-160, turn_power);
  this_thread::sleep_for(second * 2.8);

  // Move Forward a small amount
  move_straight(7, 30);
  this_thread::sleep_for(second * 1.5);

  // Turn Right to face goal zone
  turn(-48.0, turn_power);
  this_thread::sleep_for(second * 1.2);

  // Move Forward to goal zone
  move_straight(37, 20);
  this_thread::sleep_for(second * 6.5);

  // Lift Tray and place tower
  intake.interrupt();
  intake_left.stop();
  intake_right.stop();
  hinge_left.stop();
  hinge_right.stop();
  thread stack = thread(stack_tower);
  lift(760, 16);
  this_thread::sleep_for(500000);
  stack.interrupt();
  
  return 0;
}
