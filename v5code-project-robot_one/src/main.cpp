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
vex::brain Brain;

// controller instance
controller cntrlr = controller();

// motor instancnes
motor right_front = motor(PORT1, 1);
motor right_mid = motor(PORT2, 1);
motor right_back  = motor(PORT3, 1);
motor left_front = motor(PORT6, 0);
motor left_mid = motor(PORT7, 0);
motor left_back = motor(PORT8, 0);



// autonomous mode
int auton_period() {

  // 45 second timer
  while (1) {

    timer test = timer();
    test.clear();
    while (test.time(sec) < 2) {
      right_front.spin(directionType::fwd, 100.0, rpm);
      right_mid.spin(directionType::fwd, 100.0, rpm);
      right_back.spin(directionType::fwd, 100.0, rpm);
      left_front.spin(directionType::fwd, 100.0, rpm);
      left_mid.spin(directionType::fwd, 100.0, rpm);
      left_back.spin(directionType::fwd, 100.0, rpm);

      Brain.Screen.printAt(10, 50, "%d", test.time(sec));
    }

  }

  return 0;
}

// user-control mode
int main() {

    // begin autonomous period
    auton_period();
   
    // controller code
    int count = 0;
    int drive_style = 0;
    while (true) {
        Brain.Screen.printAt( 10, 50, "engage_electricity => %d", count++ );

        // if button is pressed, toggle driving mode
        if (cntrlr.ButtonB.pressing()) {
          drive_style = 0;
        }
        if (cntrlr.ButtonY.pressing()) {
          drive_style = 1;
        }
        if (cntrlr.ButtonA.pressing()) {
          drive_style = 2;
        }

        switch (drive_style) {
          case (0): // arcade style drive style
            right_front.spin(directionType::fwd, (cntrlr.Axis3.value() - cntrlr.Axis1.value()), velocityUnits::pct);
            right_mid.spin(directionType::fwd, (cntrlr.Axis3.value() - cntrlr.Axis1.value()), velocityUnits::pct);
            right_back.spin(directionType::fwd, (cntrlr.Axis3.value() - cntrlr.Axis1.value()), velocityUnits::pct);
            left_front.spin(directionType::fwd, (cntrlr.Axis3.value() + cntrlr.Axis1.value()), velocityUnits::pct);
            left_mid.spin(directionType::fwd, (cntrlr.Axis3.value() + cntrlr.Axis1.value()), velocityUnits::pct);
            left_back.spin(directionType::fwd, (cntrlr.Axis3.value() + cntrlr.Axis1.value()), velocityUnits::pct);
            break;
          case (1): // tank mode drive style
            right_front.spin(directionType::fwd, cntrlr.Axis2.value(), velocityUnits::pct);
            right_mid.spin(directionType::fwd, cntrlr.Axis2.value(), velocityUnits::pct);
            right_back.spin(directionType::fwd, cntrlr.Axis2.value(), velocityUnits::pct);
            left_front.spin(directionType::fwd, cntrlr.Axis3.value(), velocityUnits::pct);
            left_mid.spin(directionType::fwd, cntrlr.Axis3.value(), velocityUnits::pct);
            left_back.spin(directionType::fwd, cntrlr.Axis3.value(), velocityUnits::pct);
            break;
          case (2): // split arcade mode drive style (garbage)
            right_front.spin(directionType::fwd, (cntrlr.Axis2.position() - cntrlr.Axis1.position()) / (cntrlr.Axis3.value()), velocityUnits::pct);
            right_mid.spin(directionType::fwd, (cntrlr.Axis2.position() - cntrlr.Axis1.position()) / (cntrlr.Axis3.value()), velocityUnits::pct);
            right_back.spin(directionType::fwd, (cntrlr.Axis2.position() - cntrlr.Axis1.position()) / (cntrlr.Axis3.value()), velocityUnits::pct);
            left_front.spin(directionType::fwd, (cntrlr.Axis2.position() + cntrlr.Axis1.position()) / (cntrlr.Axis3.value()), velocityUnits::pct);
            left_mid.spin(directionType::fwd, (cntrlr.Axis2.position() + cntrlr.Axis1.position()) / (cntrlr.Axis3.value()), velocityUnits::pct);
            left_back.spin(directionType::fwd, (cntrlr.Axis2.position() + cntrlr.Axis1.position()) / (cntrlr.Axis3.value()), velocityUnits::pct);
            break;
        }

        // Allow other tasks to run
        this_thread::sleep_for(10);
    }

  return 0;
}
