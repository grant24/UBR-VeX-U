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

// motor instancnes
motor right_front = motor(PORT1, 1);
motor right_mid = motor(PORT2, 1);
motor right_back  = motor(PORT3, 1);
motor left_front = motor(PORT6, 0);
motor left_mid = motor(PORT7, 0);
motor left_back = motor(PORT8, 0);

// user-control mode
int main() {
   
    // timer instance
    double initial_time = Brain.timer(sec);

    while (Brain.timer(sec) - initial_time <= 45) { 
        Brain.Screen.printAt( 10, 50, "engage_electricity => %d", Brain.timer(sec)); 
        printf("time => %f\n", Brain.timer(sec) - initial_time);
        // auton tasks

        this_thread::sleep_for(10);
    }

  return 0;
}
