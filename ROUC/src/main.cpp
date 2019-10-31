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
// lift_motor motor instance to take in blocks
motor lift_motor = motor(PORT10, 1);

// user-control mode
int main() {
   
    // controller code
    int count = 0;
    bool drive_style = 1;
    bool lift_motor_loc = 0;
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
        // press button to turn lift_motor gear to other position => Up 
        bool temp = lift_motor_loc;
        if (cntrlr.ButtonUp.pressing()) {
            lift_motor_loc = !lift_motor_loc;
        }
        // button pressed, move arm to up position
        if (temp != lift_motor_loc && lift_motor_loc == 1) {
            double kp = 100;
            double ki = 10;
            double kd = 1000;
            double offset = 0;
            double integral = 0;
            double last_error = 0;
            double derivative = 0;
            while (lift_motor.rotation(deg) != -314) {
                double deg_val = lift_motor.rotation(deg);
                double error = deg_val - offset;
                integral = integral + error;
                derivative = error - last_error;
                double motor_rpm = kp*error + ki*integral + kd*derivative;
                lift_motor.spin(fwd, motor_rpm/100, rpm);
                last_error = error;
            }
        }

        // double degree = lift_motor.rotation(deg);
        // printf("motor position => %f\n", degree);

        // Allow other tasks to run
        this_thread::sleep_for(10);
    }

  return 0;
}
