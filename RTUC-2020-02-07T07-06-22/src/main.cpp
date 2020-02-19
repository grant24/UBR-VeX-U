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
motor left_front = motor(PORT10, 0);
motor left_back = motor(PORT9, 0);
motor right_front = motor(PORT20, 0);
motor right_back = motor(PORT19, 0);

/*LIFT MOTORS*/
motor left_lift = motor(PORT1, 0);
motor right_lift = motor(PORT11, 1);

/*FLIP MOTORS*/
motor claw_angle = motor(PORT3, 0);

/*CLAW MOTORS*/
motor claw_open = motor(PORT2, 0);

/*My Functions*/
void claw_open_true()
{
  claw_open.spin(directionType::fwd, 2, pct);
}
void claw_open_false()
{
  claw_open.spin(directionType::rev, 2, pct);
}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  /*Controller Code*/
  int count = 0;
  bool drive_style = 1;

  while(true)
  {
    Brain.Screen.printAt( 10, 50, "engage_electricity => %d", count++ );
    
    if(drive_style)
    {
      right_front.spin(directionType::fwd, (cntrlr.Axis1.value() - cntrlr.Axis3.value()), velocityUnits::pct);
      right_back.spin(directionType::fwd, (cntrlr.Axis1.value() - cntrlr.Axis3.value()), velocityUnits::pct);
      left_front.spin(directionType::fwd, (cntrlr.Axis1.value() + cntrlr.Axis3.value()), velocityUnits::pct);
      left_back.spin(directionType::fwd, (cntrlr.Axis1.value() + cntrlr.Axis3.value()), velocityUnits::pct);
    }

    /*CLAW ANGLE*/
    //claw_angle.spin(directionType::fwd, cntrlr.Axis2.value(), pct);
    // cntrlr.ButtonRight.pressed(claw_angle.spin(directionType::fwd));
    // cntrlr.ButtonLeft.pressed(claw_angle.spin(directionType::rev));
    if(cntrlr.ButtonRight.pressing())
    {
      claw_open.spin(directionType::fwd, 70, velocityUnits::pct);
    }
    else if(cntrlr.ButtonLeft.pressing())
    {
      claw_open.spin(directionType::rev, 70, velocityUnits::pct);
    }

    /*CLAW GRAB*/
    claw_open.spin(directionType::fwd, cntrlr.ButtonL1.pressing(), pct);
    claw_open.spin(directionType::rev, cntrlr.ButtonR1.pressing(), pct);

    /*LIFT*/
    left_lift.spin(directionType::fwd, cntrlr.ButtonX.pressing(), percentUnits::pct);
    right_lift.spin(directionType::rev, cntrlr.ButtonX.pressing(), percentUnits::pct);

    left_lift.spin(directionType::fwd, cntrlr.ButtonB.pressing(), pct);
    right_lift.spin(directionType::rev, cntrlr.ButtonB.pressing(), pct);


    printf("claw pos => %f\n", claw_angle.rotation(deg));

    this_thread::sleep_for(10);
  }
  
}
