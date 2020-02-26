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

// motor instancnes
motor right_front = motor(PORT14, 0);
motor right_mid = motor(PORT15, 0);
motor right_back  = motor(PORT16, 0);
motor left_front = motor(PORT18, 1);
motor left_mid = motor(PORT19, 1);
motor left_back = motor(PORT20, 1);
// lift_motor instance to lift stack
motor lift_motor_0 = motor(PORT9, 1);
motor lift_motor_1 = motor(PORT10, 0);
// intake motor instance to take in blocks
motor intake_left = motor(PORT4, 0);
motor intake_right = motor(PORT5, 1);
// hinge motor for intake
motor hinge_left = motor(PORT11, 1);
motor hinge_right = motor(PORT12, 0);

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
  move_straight(24, move_power);
  this_thread::sleep_for(second * 4);

  // Turn Right to approx 90 degrees
  turn(-160, turn_power);
  this_thread::sleep_for(second * 2.8);

  // Move Forward a small amount
  move_straight(24, 30);
  this_thread::sleep_for(second * 3);

  // Turn Right to face goal zone
  turn(-50, turn_power);
  this_thread::sleep_for(second * 1.2);

  // Move Forward to goal zone
  move_straight(16, 20);
  this_thread::sleep_for(second * 3.5);

  // Move Backward 1 inch
  move_straight(-1.4, move_power);
  this_thread::sleep_for(second * 1);

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

  // drive motor instances
  motor right_front = motor(PORT18, 1);
  motor right_mid = motor(PORT19, 1);
  motor right_back  = motor(PORT20, 1);
  motor left_front = motor(PORT14, 0);
  motor left_mid = motor(PORT15, 0);
  motor left_back = motor(PORT16, 0);

  // potentiometer instance
  vex::pot pot_left = vex::pot(Brain.ThreeWirePort.A);
  vex::pot pot_right = vex::pot(Brain.ThreeWirePort.B);

  // initial potentiometer positions
  double pot_left_pos = 72.8;
  double pot_right_pos = 144.2;
  // instances for lift activation
  double pot_left_poss = 72.82 - 18;
  double pot_right_poss = 144.5 + 18;

  // hinge power (percent), tolerance (int), incrementer (degree)
  double lift_pwr = 1;
  double pwr = 6;
  double tol = 0.8;
  int inc = 18;

  // initial hinge timer and engager
  double hinge_time = Brain.Timer.time(sec);
  bool hinge_engage = true;

  // initial intake timer and engager
  double intake_time = Brain.Timer.time(sec);
  bool intake_engage = true;
  
  // controller code
  int count = 0;
  bool drive_style = 1;

  // initial intake mode
  int mode = 0;

  while (true) {

      Brain.Screen.printAt( 10, 50, "engage_electricity => %d", count++ );


      // if button is pressed, toggle driving mode => X
      if (cntrlr.ButtonX.pressing()) {
          drive_style = !drive_style;
      }
      if (drive_style) {
          // arcade style drive style
          right_front.spin(directionType::fwd, (cntrlr.Axis3.value() - cntrlr.Axis1.value()), velocityUnits::pct);
          right_mid.spin(directionType::fwd, (cntrlr.Axis3.value() - cntrlr.Axis1.value()), velocityUnits::pct);
          right_back.spin(directionType::fwd, (cntrlr.Axis3.value() - cntrlr.Axis1.value()), velocityUnits::pct);
          left_front.spin(directionType::fwd, (cntrlr.Axis3.value() + cntrlr.Axis1.value()), velocityUnits::pct);
          left_mid.spin(directionType::fwd, (cntrlr.Axis3.value() + cntrlr.Axis1.value()), velocityUnits::pct);
          left_back.spin(directionType::fwd, (cntrlr.Axis3.value() + cntrlr.Axis1.value()), velocityUnits::pct);
      }
      else {
          // tank mode drive style
          right_front.spin(directionType::fwd, cntrlr.Axis2.value(), velocityUnits::pct);
          right_mid.spin(directionType::fwd, cntrlr.Axis2.value(), velocityUnits::pct);
          right_back.spin(directionType::fwd, cntrlr.Axis2.value(), velocityUnits::pct);
          left_front.spin(directionType::fwd, cntrlr.Axis3.value(), velocityUnits::pct);
          left_mid.spin(directionType::fwd, cntrlr.Axis3.value(), velocityUnits::pct);
          left_back.spin(directionType::fwd, cntrlr.Axis3.value(), velocityUnits::pct);
      }


      // lift motor - active when R1 is held - move arms out of the way
      while (cntrlr.ButtonR1.pressing()) {
        lift_motor_0.spin(fwd, cntrlr.Axis2.value()/3, velocityUnits::pct);
        lift_motor_1.spin(fwd, cntrlr.Axis2.value()/3, velocityUnits::pct);

        right_front.spin(directionType::fwd, cntrlr.Axis3.value()/8, velocityUnits::pct);
        right_mid.spin(directionType::fwd, cntrlr.Axis3.value()/8, velocityUnits::pct);
        right_back.spin(directionType::fwd, cntrlr.Axis3.value()/8, velocityUnits::pct);
        left_front.spin(directionType::fwd, cntrlr.Axis3.value()/8, velocityUnits::pct);
        left_mid.spin(directionType::fwd, cntrlr.Axis3.value()/8, velocityUnits::pct);
        left_back.spin(directionType::fwd, cntrlr.Axis3.value()/8, velocityUnits::pct);

        if ((pot_left.value(deg) > pot_left_poss - tol) && (pot_left.value(deg) < pot_left_poss + tol)) { //if within tolerance, dont move 
          hinge_left.spin(fwd, 0, pct);
        } 
        else if (pot_left.value(deg) > pot_left_poss) { //move out
          hinge_left.spin(fwd, -lift_pwr, pct);
        }
        else if (pot_left.value(deg) < pot_left_poss) //move in
        {
          hinge_left.spin(fwd, lift_pwr, pct);
        }
        // right
        if ((pot_right.value(deg) < pot_right_poss + tol) && (pot_right.value(deg) > pot_right_poss - tol)) { // if within tolerance, dont move 
          hinge_right.spin(fwd, 0, pct);
        } 
        else if (pot_right.value(deg) > pot_right_poss) { //move out
          hinge_right.spin(fwd, lift_pwr, pct);
        }
        else if (pot_right.value(deg) < pot_right_poss) //move in
        {
          hinge_right.spin(fwd, -lift_pwr, pct);
        }
      }
      lift_motor_0.spin(fwd, 0, velocityUnits::pct);
      lift_motor_1.spin(fwd, 0, velocityUnits::pct);


      // hinge motor position management
      // left
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

      // hinge motor degree incrementation
      // pressing Up to squeeze more
      if (cntrlr.ButtonUp.pressing() && hinge_engage == true) {
        pot_left_pos += inc;
        pot_right_pos -= inc;
        hinge_engage = false;
        hinge_time = Brain.Timer.time(sec);
      }
      // pressing Down to squeeze less
      else if (cntrlr.ButtonDown.pressing() && hinge_engage == true) {
        pot_left_pos -= inc;
        pot_right_pos += inc;
        hinge_engage = false;
        hinge_time = Brain.Timer.time(sec);
      }

      // cooldown for hinge increment button (0.5 seconds)
      if (Brain.Timer.time(sec) - hinge_time > 0.5 && hinge_engage == false) {
        hinge_engage = true;
        hinge_time = Brain.Timer.time(sec);
      }


      // intake Control for picking up cubes
      switch (mode) {
        // stopped
        case 0: intake_left.spin(fwd, 0, velocityUnits::pct);
                intake_right.spin(fwd, 0, velocityUnits::pct);
                if (cntrlr.ButtonA.pressing() && intake_engage == true) {
                  mode = 1;
                  intake_engage = false;
                  intake_time = Brain.Timer.time(sec);
                }
                if (cntrlr.ButtonB.pressing() && intake_engage == true) {
                  mode = 2;
                  intake_engage = false;
                  intake_time = Brain.Timer.time(sec);
                }
                break;
        // sucking
        case 1:	intake_left.spin(fwd, 100, velocityUnits::pct);
                intake_right.spin(fwd, 100, velocityUnits::pct);
                if (cntrlr.ButtonA.pressing() && intake_engage == true) {
                  mode = 0;
                  intake_engage = false;
                  intake_time = Brain.Timer.time(sec);
                }
                if (cntrlr.ButtonB.pressing() && intake_engage == true) {
                  mode = 2;
                  intake_engage = false;
                  intake_time = Brain.Timer.time(sec);
                }
                break;
        // spitting
        case 2: intake_left.spin(fwd, -100, velocityUnits::pct);
                intake_right.spin(fwd, -100, velocityUnits::pct);
                if (cntrlr.ButtonA.pressing() && intake_engage == true) {
                  mode = 1;
                  intake_engage = false;
                  intake_time = Brain.Timer.time(sec);
                }
                if (cntrlr.ButtonB.pressing() && intake_engage == true) {
                  mode = 0;
                  intake_engage = false;
                  intake_time = Brain.Timer.time(sec);
                }
                break;
      }

      // cooldown for intake (0.5 seconds)
      if (Brain.Timer.time(sec) - intake_time > 0.5 && intake_engage == false) {
        intake_engage = true;
        intake_time = Brain.Timer.time(sec);
      }


      // pot test
      printf("pot_left position => %f     |     ", pot_left.value(deg));
      printf("pot_right position => %f\n", pot_right.value(deg));

      // Allow other tasks to run
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
