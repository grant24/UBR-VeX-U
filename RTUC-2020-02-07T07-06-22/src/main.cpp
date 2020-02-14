/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\coben                                            */
/*    Created:      Fri Feb 07 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain Brain;
vex::controller cntrlr;

// define your global instances of motors and other devices here
motor main_motor_left = motor(PORT1, 1);
motor main_motor_right = motor(PORT11, 0);
motor claw_angle = motor(PORT3, 0);
motor claw_open = motor(PORT4, 0);

int main() {
    int count = 0;
   
    while(1) {
        Brain.Screen.printAt( 10, 50, "Hello V5 %d", count++ );
        // Allow other tasks to run

        main_motor_left.spin(fwd, cntrlr.Axis2.value(), pct);
        main_motor_right.spin(fwd, cntrlr.Axis2.value(), pct);

        claw_angle.spin(fwd, cntrlr.Axis3.value(), pct);

        this_thread::sleep_for(10);
    }
}
