/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
controller cntrlr = controller();
/*DRIVE MOTORS*/
motor left_front = motor(PORT8, 1);
motor left_back = motor(PORT9, 0);
motor right_front = motor(PORT18, 1);
motor right_back = motor(PORT19, 0);

/*LIFT MOTORS*/
motor left_lift = motor(PORT6, 0);
motor right_lift = motor(PORT5, 1);
bool lift_bool = true;
double lift_time = Brain.Timer.time(sec);
int lift_mode = 0;

/*FLIP MOTORS*/
motor claw_angle = motor(PORT2, 0);
bool claw_angle_bool = true;
double claw_time = Brain.Timer.time(sec);
int claw_mode = 0;

/*CLAW MOTORS*/
motor grab = motor(PORT1, 0);
bool grab_bool = true;
double grab_time = Brain.Timer.time(sec);
int grab_mode = 0;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/* MAINTAIN POSITION */
void maintain_claw(double position) {
  double tol = 25;
  double pwr = 10;
  if ((claw_angle.rotation(deg) > position - tol) && (claw_angle.rotation(deg) < position + tol)) {
    claw_angle.spin(fwd, 0, pct);
  }
  else if (claw_angle.rotation(deg) < position) {
    claw_angle.spin(fwd, pwr, pct);
  }
  else if (claw_angle.rotation(deg) > position) {
    claw_angle.spin(fwd, -pwr, pct);
  }
}

/* MAINTAIN POSITION */
void maintain_lift(double position_0, double position_1) {
  double tol = 25;
  double pwr = 25;
  if ((left_lift.rotation(deg) < position_0 + tol) && (left_lift.rotation(deg) > position_0 - tol)) {
    left_lift.spin(fwd, 0, pct);
  }
  else if (left_lift.rotation(deg) > position_0) {
    left_lift.spin(fwd, -pwr, pct);
  }
  else if (left_lift.rotation(deg) < position_0) {
    left_lift.spin(fwd, pwr, pct);
  }
  if ((right_lift.rotation(deg) < position_1 + tol) && (right_lift.rotation(deg) > position_1 - tol)) {
    right_lift.spin(fwd, 0, pct);
  }
  else if (right_lift.rotation(deg) > position_1) {
    right_lift.spin(fwd, -pwr, pct);
  }
  else if (right_lift.rotation(deg) < position_1) {
    right_lift.spin(fwd, pwr, pct);
  }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  left_lift.startRotateFor(-200, rotationUnits::deg, 100, velocityUnits::pct);
  right_lift.startRotateFor(-200, rotationUnits::deg, 100, velocityUnits::pct);

  this_thread::sleep_for(10);
  
  left_front.startRotateFor(400, rotationUnits::deg, 100, velocityUnits::pct);
  right_front.startRotateFor(-400, rotationUnits::deg, 100, velocityUnits::pct);
  left_back.startRotateFor(400, rotationUnits::deg, 100, velocityUnits::pct);
  right_back.startRotateFor(-400, rotationUnits::deg, 100, velocityUnits::pct);

  this_thread::sleep_for(1000);

  left_front.startRotateFor(-200, rotationUnits::deg, 100, velocityUnits::pct);
  right_front.startRotateFor(200, rotationUnits::deg, 100, velocityUnits::pct);
  left_back.startRotateFor(-200, rotationUnits::deg, 100, velocityUnits::pct);
  right_back.startRotateFor(200, rotationUnits::deg, 100, velocityUnits::pct);
  
  this_thread::sleep_for(10);

  left_lift.startRotateFor(-200, rotationUnits::deg, 100, velocityUnits::pct);
  right_lift.startRotateFor(-200, rotationUnits::deg, 100, velocityUnits::pct);

  
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  double claw_angle_deg = claw_angle.rotation(deg);

  double left_lift_deg = left_lift.rotation(deg);
  double right_lift_deg = right_lift.rotation(deg);

  bool engage = true;
  bool claw_mode = false;
  float curr_time;

  /*Controller Code*/
  int count = 0;

  while(true)
  {
    Brain.Screen.printAt( 10, 50, "engage_electricity => %d", count++ );
    
    //Drive
    right_front.spin(directionType::fwd, (cntrlr.Axis1.value() - cntrlr.Axis3.value()), velocityUnits::pct);
    right_back.spin(directionType::fwd, (cntrlr.Axis1.value() - cntrlr.Axis3.value()), velocityUnits::pct);
    left_front.spin(directionType::fwd, (cntrlr.Axis1.value() + cntrlr.Axis3.value()), velocityUnits::pct);
    left_back.spin(directionType::fwd, (cntrlr.Axis1.value() + cntrlr.Axis3.value()), velocityUnits::pct);

    //CLAW ANGLE
    if (cntrlr.ButtonL1.pressing()) {
      claw_angle_deg += 1;  
    }
    if (cntrlr.ButtonL2.pressing()) {
      claw_angle_deg -= 1;
    }
    maintain_claw(claw_angle_deg);

    //LIFT ANGLE
    if (cntrlr.ButtonR2.pressing()) {
      left_lift_deg += 3;
      right_lift_deg += 3;
    }
    else if (cntrlr.ButtonR1.pressing()) {
      left_lift_deg -= 3;
      right_lift_deg -= 3;
    }
    maintain_lift(left_lift_deg, right_lift_deg);

    //CLAW GRAB
    if (cntrlr.ButtonA.pressing() && engage == true) {
      claw_mode = !claw_mode;
      engage = false;
      curr_time = Brain.Timer.time(sec);
    }
    if (Brain.Timer.time(sec) - curr_time > .5 && engage == false) {
      engage = true;
    }

    if (claw_mode) {
      grab.spin(fwd, -100, pct);
    }
    else if (!claw_mode) {
      grab.spin(fwd, 5, pct);
    }

    this_thread::sleep_for(10);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
