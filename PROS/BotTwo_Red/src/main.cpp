#include "main.h"


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	// Building the chassis
	std::shared_ptr<ChassisController> chassis = ChassisControllerBuilder()
		.withMotors( {-8, 9}, {18, -19})
		.withDimensions(AbstractMotor::gearset::green, {{4_in, 13_in}, imev5GreenTPR})
		.build();

		// Lift Motor controller & variables
		const int left_lift = 6;
		const int right_lift = 5;
		const int num_heights = 3;
		const int heights[num_heights] = {0, 680, 4200};
		ControllerButton R1(ControllerDigital::R1);
		ControllerButton R2(ControllerDigital::R2);
		std::shared_ptr<AsyncPositionController<double, double>> lift_control =
	  	AsyncPosControllerBuilder()
				.withMotor({-left_lift, right_lift})
				.build();
		int goal_height = 0;

		// Claw Angle controller & variables
		const int claw_angle = 2;
		const int inc = 30;
		const int num_height = 3;
		const int height[num_height] = {0, -350, -600};
		int angle = 0;
		ControllerButton A(ControllerDigital::A);
		ControllerButton B(ControllerDigital::B);
		std::shared_ptr<AsyncPositionController<double, double>> angle_control =
		  AsyncPosControllerBuilder()
				.withMotor(claw_angle)
				.build();

		// Claw Grab controller & variables
		const int claw_grab = 1;
		const double claw_open = 220;
		const double claw_closed = 30;
		bool engage = false;
		ControllerButton L1(ControllerDigital::L1);
		ControllerButton L2(ControllerDigital::L2);
		std::shared_ptr<AsyncPositionController<double, double>> grab_control =
			AsyncPosControllerBuilder()
				.withMotor(claw_grab)
				.build();

		// Auton Movement
		lift_control->setTarget(heights[1] - 280);

		chassis->moveDistance(30_in);
		chassis->waitUntilSettled();

		chassis->moveDistance(-20_in);
		chassis->waitUntilSettled();
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, th]
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	Controller cntrlr;

	// Building the chassis
	std::shared_ptr<ChassisController> drive = ChassisControllerBuilder()
		.withMotors( {-8, 9}, {18, -19})
		.withDimensions(AbstractMotor::gearset::green, {{4_in, 13_in}, imev5GreenTPR})
		.build();

	// Lift Motor controller & variables
	const int left_lift = 6;
	const int right_lift = 5;
	const int num_heights = 4;
	const int heights[num_heights] = {0, 260, 680, 4200};
	ControllerButton R1(ControllerDigital::R1);
	ControllerButton R2(ControllerDigital::R2);
	std::shared_ptr<AsyncPositionController<double, double>> lift_control =
  	AsyncPosControllerBuilder()
			.withMotor({-left_lift, right_lift})
			.build();
	int goal_height = 0;
	ControllerButton UP(ControllerDigital::up);
	ControllerButton DOWN(ControllerDigital::down);
	int lift_angle;
	int lift_inc = 100;

	// Claw Angle controller & variables
	const int claw_angle = 2;
	const int inc = 30;
	const int num_height = 4;
	const int height[num_height] = {0, -20, -600, -1000};
	int angle = 0;
	ControllerButton X(ControllerDigital::X);
	ControllerButton B(ControllerDigital::B);
	std::shared_ptr<AsyncPositionController<double, double>> angle_control =
	  AsyncPosControllerBuilder()
			.withMotor(claw_angle)
			.build();

	// Claw Grab controller & variables
	const int claw_grab = 3;
	const double claw_open = 220;
	const double claw_closed = 0;
	ControllerButton L1(ControllerDigital::L1);
	ControllerButton L2(ControllerDigital::L2);
	std::shared_ptr<AsyncPositionController<double, double>> grab_control =
		AsyncPosControllerBuilder()
			.withMotor(claw_grab)
			.build();

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		// Setting Chassis to arcade drive
		if (goal_height == 3) {
			drive->getModel()->arcade(cntrlr.getAnalog(ControllerAnalog::leftY ) / 3,
																cntrlr.getAnalog(ControllerAnalog::rightX) / 3);
		} else {
			drive->getModel()->arcade(cntrlr.getAnalog(ControllerAnalog::leftY),
																cntrlr.getAnalog(ControllerAnalog::rightX));
		}

		// Lifting Arm to set position
		if (R1.changedToPressed() && goal_height < num_heights - 1) {
      goal_height++;
      lift_control->setTarget(heights[goal_height]);
			lift_angle = heights[goal_height];
			angle_control->setTarget(height[goal_height]);
			angle = height[goal_height];
    } else if (R2.changedToPressed() && goal_height > 0) {
      goal_height--;
      lift_control->setTarget(heights[goal_height]);
			lift_angle = heights[goal_height];
			angle_control->setTarget(height[goal_height]);
			angle = height[goal_height];
    }

		// Angling Claw to set position
		if (X.changedToPressed()) {
			angle += inc;
      angle_control->setTarget(angle);
    } else if (B.changedToPressed()) {
			angle -= inc;
      angle_control->setTarget(angle);
    }

		// Open/Close Claw to grab cube
		if (L1.changedToPressed()) {
			grab_control->setTarget(claw_open);
		}
		else if (L2.changedToPressed()) {
			grab_control->setTarget(claw_closed);
		}

		// Increment Lift from current position
		if (UP.changedToPressed()) {
			lift_control->setTarget(lift_angle + lift_inc);
			lift_angle += lift_inc;
		}
		if (DOWN.changedToPressed()) {
			lift_control->setTarget(lift_angle - lift_inc);
			lift_angle -= lift_inc;
		}

		// Motor claw_grab_motor = Motor(claw_angle);
		// printf("claw grab position => %f\n", claw_grab_motor.getPosition());

		pros::delay(10);
	}
}
