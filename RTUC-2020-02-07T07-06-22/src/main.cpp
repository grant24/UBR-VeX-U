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
bool lift_bool = true;
double lift_time = Brain.Timer.time(sec);
int lift_mode = 0;

/*FLIP MOTORS*/
motor claw_angle = motor(PORT2, 0);
bool claw_angle_bool = true;
double claw_time = Brain.Timer.time(sec);
int claw_mode = 0;

/*CLAW MOTORS*/
motor grab = motor(PORT1, 0);
bool grab_bool = true;
double grab_time = Brain.Timer.time(sec);
int grab_mode = 0;

/* MAINTAIN POSITION */
void maintain_claw(double position) {
  double tol = 2;
  double pwr = 8;
  if ((claw_angle.rotation(deg) > position - tol) && (claw_angle.rotation(deg) < position + tol)) {
    claw_angle.spin(fwd, 0, pct);
  }
  else if (claw_angle.rotation(deg) < position) {
    left_lift.spin(fwd, 0, pct);
    right_lift.spin(fwd, 0, pct);
  }
  else if (claw_angle.rotation(deg) > position) {
    claw_angle.spin(directionType::rev, pwr, pct);
  }
}

/* MAINTAIN POSITION */
void maintain_lift(double position_0, double position_1) {
  double tol = 3;
  double pwr = 15;
  if ((left_lift.rotation(deg) > position_0 + tol) && (left_lift.rotation(deg) < position_0 - tol)) {
    left_lift.spin(fwd, 0, pct);
    right_lift.spin(fwd, 0, pct);
  }
  else if (left_lift.rotation(deg) > position_0) {
    left_lift.spin(fwd, pwr, pct);
    right_lift.spin(fwd, pwr, pct);
  }
  else if (left_lift.rotation(deg) < position_0) {
    left_lift.spin(fwd, -pwr, pct);
    right_lift.spin(fwd, -pwr, pct);
  }
  if ((right_lift.rotation(deg) > position_1 + tol) && (right_lift.rotation(deg) < position_1 - tol)) {
    left_lift.spin(fwd, 0, pct);
    right_lift.spin(fwd, 0, pct);
  }
  else if (right_lift.rotation(deg) > position_1) {
    left_lift.spin(fwd, pwr, pct);
    right_lift.spin(fwd, pwr, pct);
  }
  else if (right_lift.rotation(deg) < position_1) {
    left_lift.spin(fwd, -pwr, pct);
    right_lift.spin(fwd, -pwr, pct);
  }
}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  double claw_angle_deg = claw_angle.rotation(deg);

  double left_lift_deg = left_lift.rotation(deg);
  double right_lift_deg = right_lift.rotation(deg);

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

    //CLAW ANGLE
    if (cntrlr.ButtonL1.pressing()) {
      claw_angle_deg += 1;  
    }
    if (cntrlr.ButtonL2.pressing()) {
      claw_angle_deg -= 1;
    }
    maintain_claw(claw_angle_deg);

    //LIFT ANGLE
    if (cntrlr.ButtonR2.pressing()) {
      left_lift_deg += 1;
      right_lift_deg += 1;
    }
    else if (cntrlr.ButtonR1.pressing()) {
      left_lift_deg -= 1;
      right_lift_deg -= 1;
    }
    maintain_lift(left_lift_deg, right_lift_deg);

    // //CLAW GRAB
    // switch(grab_mode)
    // {
    //   //stopped
    //   case 0: grab.spin(directionType::fwd, 0, velocityUnits::pct);
    //           if(cntrlr.ButtonL2.pressing() && claw_angle_bool == true)
    //           {
    //             grab_mode = 1;
    //             grab_bool = false;
    //             grab_time = Brain.Timer.time(sec);
    //           }
    //           if(cntrlr.ButtonR2.pressing() && claw_angle_bool == true)
    //           {
    //             grab_mode = 2;
    //             grab_bool = false;
    //             grab_time = Brain.Timer.time(sec);
    //           }
    //           break;

    //   //GRAB
    //   case 1: grab.spin(directionType::fwd, 10, velocityUnits::pct);
    //           if(cntrlr.ButtonL2.pressing() && claw_angle_bool == true)
    //           {
    //             grab_mode = 0;
    //             grab_bool = false;
    //             grab_time = Brain.Timer.time(sec);
    //           }
    //           if(cntrlr.ButtonR2.pressing() && claw_angle_bool == true)
    //           {
    //             grab_mode = 2;
    //             grab_bool = false;
    //             grab_time = Brain.Timer.time(sec);
    //           }
    //           break;

    //   //RELEASE
    //   case 2:grab.spin(directionType::rev, 10, velocityUnits::pct);
    //           if(cntrlr.ButtonL2.pressing() && claw_angle_bool == true)
    //           {
    //             grab_mode = 1;
    //             grab_bool = false;
    //             grab_time = Brain.Timer.time(sec);
    //           }
    //           if(cntrlr.ButtonR2.pressing() && claw_angle_bool == true)
    //           {
    //             grab_mode = 0;
    //             grab_bool = false;
    //             grab_time = Brain.Timer.time(sec);
    //           }
    //           break;

    // }
    //       // cooldown for claw grab (0.5 seconds)
    //   if (Brain.Timer.time(sec) - grab_time > 0.5 && grab_bool == false) {
    //     grab_bool = true;
    //   }


    // //Lift
    // switch(lift_mode)
    // {
    //   //stopped
    //   case 0: left_lift.spin(directionType::fwd, 0, velocityUnits::pct);
    //           right_lift.spin(directionType::fwd, 0, velocityUnits::pct);
    //           if(cntrlr.ButtonX.pressing() && claw_angle_bool == true)
    //           {
    //             lift_mode = 1;
    //             lift_bool = false;
    //             lift_time = Brain.Timer.time(sec);
    //           }
    //           if(cntrlr.ButtonB.pressing() && claw_angle_bool == true)
    //           {
    //             lift_mode = 2;
    //             lift_bool = false;
    //             lift_time = Brain.Timer.time(sec);
    //           }
    //           break;

    //   //UP
    //   case 1: left_lift.spin(directionType::fwd, 100, velocityUnits::pct);
    //           right_lift.spin(directionType::fwd, 100, velocityUnits::pct);
    //           if(cntrlr.ButtonX.pressing() && claw_angle_bool == true)
    //           {
    //             lift_mode = 0;
    //             lift_bool = false;
    //             lift_time = Brain.Timer.time(sec);
    //           }
    //           if(cntrlr.ButtonB.pressing() && claw_angle_bool == true)
    //           {
    //             lift_mode = 2;
    //             lift_bool = false;
    //             lift_time = Brain.Timer.time(sec);
    //           }
    //           break;

    //   //DOWN
    //   case 2: left_lift.spin(directionType::rev, 100, velocityUnits::pct);
    //           right_lift.spin(directionType::rev, 100, velocityUnits::pct);
    //           if(cntrlr.ButtonX.pressing() && claw_angle_bool == true)
    //           {
    //             lift_mode = 1;
    //             lift_bool = false;
    //             lift_time = Brain.Timer.time(sec);
    //           }
    //           if(cntrlr.ButtonB.pressing() && claw_angle_bool == true)
    //           {
    //             lift_mode = 0;
    //             lift_bool = false;
    //             lift_time = Brain.Timer.time(sec);
    //           }
    //           break;

    // }
    //       // cooldown for intake (0.5 seconds)
    //   if (Brain.Timer.time(sec) - lift_time > 0.5 && lift_bool == false) {
    //     lift_mode = true;
    //   }
    
    /*CLAW ANGLE*/
    printf("claw angle (deg) => %f\n", claw_angle.rotation(deg));
    //claw_angle.spin(directionType::fwd, (cntrlr.Axis2.value()), velocityUnits::pct);

    /*CLAW GRAB*/
    printf("claw grab (deg) => %f\n", grab.rotation(deg));
    
    /*LIFT*/
    printf("left lift (deg) => %f\n", left_lift.rotation(deg));
    printf("right lift (deg) => %f\n\n", right_lift.rotation(deg));
    this_thread::sleep_for(10);
  }
  
}
