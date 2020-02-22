/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\coben                                            */
/*    Created:      Fri Feb 07 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "robot-config.h"

using namespace vex;

controller cntrlr = controller();

/*DRIVE MOTORS*/
motor left_front = motor(PORT8, 1);
motor left_back = motor(PORT9, 0);
motor right_front = motor(PORT18, 1);
motor right_back = motor(PORT19, 0);

/*LIFT MOTORS*/
motor left_lift = motor(PORT6, 0);
motor right_lift = motor(PORT5, 1);
bool lift_bool = false;

/*FLIP MOTORS*/
motor claw_angle = motor(PORT2, 0);
bool claw_angle_bool = false;

/*CLAW MOTORS*/
motor claw_grab = motor(PORT1, 0);
bool grab_bool = false;


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

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

    //Claw Angle
    if(cntrlr.ButtonUp.pressing())
    {
      claw_angle.spin(directionType::fwd);
      claw_angle_bool = true;
    }
    else if (cntrlr.ButtonDown.pressing())
    {
      claw_angle.spin(directionType::rev);
      claw_angle_bool = true;
    }
    else if((cntrlr.ButtonUp.pressing() || cntrlr.ButtonDown.pressing()) && claw_angle_bool == true)
    {
      claw_angle.spin(directionType::fwd, 0, velocityUnits::pct);
      claw_angle_bool = false;
    }
    //Claw Grab
    if(cntrlr.ButtonL2.pressing())
    {
      claw_grab.spin(directionType::fwd);
      grab_bool = true;
    }
    else if(cntrlr.ButtonR2.pressing())
    {
      claw_grab.spin(directionType::rev);
      grab_bool = true;
    }
    else if((cntrlr.ButtonL2.pressing() || cntrlr.ButtonR2.pressing()) && grab_bool == true)
    {
      claw_grab.spin(directionType::fwd, 0, velocityUnits::pct);
      grab_bool = false;
    }
    //LIFT
    if(cntrlr.ButtonX.pressing())
    {
      right_lift.spin(directionType::fwd);
      left_lift.spin(directionType::fwd);
      lift_bool = true;
    }
    else if(cntrlr.ButtonB.pressing())
    {
      right_lift.spin(directionType::rev);
      left_lift.spin(directionType::rev);
      lift_bool = true;
    }
    else if((cntrlr.ButtonX.pressing() || cntrlr.ButtonB.pressing()) && lift_bool == true)
    {
      right_lift.spin(directionType::fwd, 0, velocityUnits::pct);
      left_lift.spin(directionType::fwd, 0, velocityUnits::pct);
      lift_bool = false;
    }
    
    /*CLAW ANGLE*/
    printf("claw angle (deg) => %f\n", claw_angle.rotation(deg));
    //claw_angle.spin(directionType::fwd, (cntrlr.Axis2.value()), velocityUnits::pct);

    /*CLAW GRAB*/
    printf("claw grab (deg) => %f\n", claw_grab.rotation(deg));
    
    /*LIFT*/
    printf("left lift (deg) => %f\n", left_lift.rotation(deg));
    printf("right lift (deg) => %f\n\n", right_lift.rotation(deg));
    this_thread::sleep_for(10);
  }
  
}
