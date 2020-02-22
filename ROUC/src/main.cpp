// ---- START VEXCODE CONFIGURED DEVICES ----
// // Robot Configuration:
// // [Name]               [Type]        [Port(s)]
// //
// // ---- END VEXCODE CONFIGURED DEVICES ----
// /*----------------------------------------------------------------------------*/
// /*                                                                            */
// /*    Module:       main.cpp                                                  */
// /*    Author:       C:\Users\grega                                            */
// /*    Created:      Wed Oct 09 2019                                           */
// /*    Description:  V5 project                                                */
// /*                                                                            */
// /*----------------------------------------------------------------------------*/
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;


// controller instance
controller cntrlr = controller();

// drive motor instances
motor right_front = motor(PORT18, 1);
motor right_mid = motor(PORT19, 1);
motor right_back  = motor(PORT20, 1);
motor left_front = motor(PORT14, 0);
motor left_mid = motor(PORT15, 0);
motor left_back = motor(PORT16, 0);
// lift_motor instance to lift stack
motor lift_motor_0 = motor(PORT9, 1);
motor lift_motor_1 = motor(PORT10, 0);
// intake motor instance to take in blocks
motor intake_left = motor(PORT4, 0);
motor intake_right = motor(PORT5, 1);
// hinge motor for intake
motor hinge_left = motor(PORT11, 1);
motor hinge_right = motor(PORT12, 0);


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

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

  return 0;
}
