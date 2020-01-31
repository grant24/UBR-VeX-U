/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\grega                                            */
/*    Created:      Wed Oct 09 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;


// A global instance of vex::brain used for printing to the V5 brain screen
brain Brain;

// controller instance
controller cntrlr = controller();

// drive motor instances
motor right_front = motor(PORT1, 1);
motor right_mid = motor(PORT2, 1);
motor right_back  = motor(PORT3, 1);
motor left_front = motor(PORT6, 0);
motor left_mid = motor(PORT7, 0);
motor left_back = motor(PORT8, 0);
// lift_motor instance to lift stack
motor lift_motor = motor(PORT10, 1);
// intake motor instance to take in blocks
motor intake_left = motor(PORT4, 1);
motor intake_right = motor(PORT5, 0);


// user-control mode
int main() {

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

        // lift motor
        lift_motor.spin(fwd, cntrlr.Axis2.value(), velocityUnits::pct);        

        // Intake Control for picking up cubes */
		    switch (mode) {
          // stopped
          case 0: intake_left.spin(fwd, double voltage, voltageUnits units);
                  intake_right.moveVoltage(0);
                  if (suck.changedToPressed()) {
                    mode = 1;
                  }
                  else if (spit.changedToPressed()) {
                    mode = 2;
                  }
                  break;
          // sucking
          case 1:	intake_left.moveVoltage(value);
                  intake_right.moveVoltage(value);
                  if (suck.changedToPressed()) {
                    mode = 0;
                  }
                  else if (spit.changedToPressed()) {
                    mode = 2;
                  }
                  break;
          // spitting
          case 2: intake_left.moveVoltage(-value);
                  intake_right.moveVoltage(-value);
                  if (suck.changedToPressed()) {
                    mode = 1;
                  }
                  else if (spit.changedToPressed()) {
                    mode = 0;
                  }
                  break;
        }

        // output to get motor arm position in degrees
        double degree = lift_motor.rotation(deg);
        printf("motor position => %f\n", degree);

        // Allow other tasks to run
        this_thread::sleep_for(10);
    }

  return 0;
}
