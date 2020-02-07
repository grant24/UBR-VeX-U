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
  
  // controller code
  int count = 0;
  bool drive_style = 1;

  // initial intake mode
  int mode = 0;

  // initial hinge power and timer and engager
  double pwr = 0;
  double curr_time = Brain.Timer.time(sec);
  bool engage = true;

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

      // lift motor - active when holding right arrow
      while (cntrlr.ButtonRight.pressing()) {
        lift_motor_0.spin(fwd, cntrlr.Axis2.value()/3, velocityUnits::pct);
        lift_motor_1.spin(fwd, cntrlr.Axis2.value()/3, velocityUnits::pct);      
      }
      lift_motor_0.spin(fwd, 0, velocityUnits::pct);
        lift_motor_1.spin(fwd, 0, velocityUnits::pct);

      // hinge motor - increment power if up or down is pressed
      if (cntrlr.ButtonUp.pressing() && pwr < 100 && engage == true) {
        pwr += 1.0;
        printf("power percentage => %f\n", pwr);
        engage = false;
      }
      else if (cntrlr.ButtonDown.pressing() && pwr > -100 && engage == true) {
        pwr -= 1.0;
        printf("power percentage => %f\n", pwr);
        engage = false;
      }
      else if (cntrlr.ButtonLeft.pressing() && pwr > -100) {
        pwr = 0;
        printf("power percentage => %f\n", pwr);
      }
      hinge_left.spin(fwd, pwr, pct);
      hinge_right.spin(fwd, pwr, pct);

      // cooldown for hinge (0.5 seconds)
      if (Brain.Timer.time(sec) - curr_time > 0.5 && engage == false) {
        engage = true;
        curr_time = Brain.Timer.time(sec);
      }

      // Intake Control for picking up cubes
      switch (mode) {
        // stopped
        case 0: intake_left.spin(fwd, 0, velocityUnits::pct);
                intake_right.spin(fwd, 0, velocityUnits::pct);
                if (cntrlr.ButtonA.pressing()) {
                  mode = 1;
                }
                if (cntrlr.ButtonB.pressing()) {
                  mode = 2;
                }
                break;
        // sucking
        case 1:	intake_left.spin(fwd, 100, velocityUnits::pct);
                intake_right.spin(fwd, 100, velocityUnits::pct);
                if (cntrlr.ButtonA.pressing()) {
                  mode = 0;
                }
                if (cntrlr.ButtonB.pressing()) {
                  mode = 2;
                }
                break;
        // spitting
        case 2: intake_left.spin(fwd, -100, velocityUnits::pct);
                intake_right.spin(fwd, -100, velocityUnits::pct);
                if (cntrlr.ButtonA.pressing()) {
                  mode = 1;
                }
                if (cntrlr.ButtonB.pressing()) {
                  mode = 0;
                }
                break;
      }

      // output to get motor arm position in degrees
      // double degree = lift_motor_0.rotation(deg);
      // printf("motor position => %f\n", degree);

      // Allow other tasks to run
      this_thread::sleep_for(10);
  }

  return 0;
}
