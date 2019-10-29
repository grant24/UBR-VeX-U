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
// flapper motor instance to take in blocks
motor flapper = motor(PORT10, 0);

// user-control mode
int main() {
   
    // controller code
    int count = 0;
    bool drive_style = 1;
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

        // start at 0, end at -314 degrees
        // hold button to turn flapper gear => R1
        while (cntrlr.ButtonR1.pressing()) {
          flapper.spin(fwd, 10, pct);
        }
        flapper.stop();
        // test R2 to see motor direction
        while (cntrlr.ButtonR2.pressing()) {
          flapper.spin(directionType::rev, 10, pct);
        }
        flapper.stop();

        // double degree = flapper.rotation(deg);
        // printf("motor position => %f\n", degree);

        // Allow other tasks to run
        this_thread::sleep_for(10);
    }

  return 0;
}
