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
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	Controller cntrlr;

	// Motor hinge_left = Motor(12);
	// Motor hinge_right = Motor(11);

	// Building the chassis
	std::shared_ptr<ChassisController> drive = ChassisControllerBuilder()
		.withMotors( {-18, -19, -20}, {14, 15, 16})
		.withDimensions(AbstractMotor::gearset::green, {{4_in, 18_in}, imev5GreenTPR})
		.build();

	// Hinge Position Controller
	const int hinge_left = 12;
	const int hinge_right = 11;
	const int num_pos = 3;
	const int hinge_pos_left[num_pos] = {0, 0, 103};
	const int hinge_pos_right[num_pos] = {0, 0, -103};
	ControllerButton UP(ControllerDigital::up);
	ControllerButton DOWN(ControllerDigital::down);
	std::shared_ptr<AsyncPositionController<double, double>> hinge_left_control =
		AsyncPosControllerBuilder()
			.withMotor(hinge_left)
			.build();
	std::shared_ptr<AsyncPositionController<double, double>> hinge_right_control =
		AsyncPosControllerBuilder()
			.withMotor(hinge_right)
			.build();
	int goal_position = 1;

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		// Setting drive mode
	  drive->getModel()->arcade(cntrlr.getAnalog(ControllerAnalog::leftY),
														  cntrlr.getAnalog(ControllerAnalog::rightX));

		// Maintiain Hinge position
		if (UP.changedToPressed() && goal_position < num_pos-1) {
			goal_position++;
			hinge_left_control->setTarget(hinge_pos_left[goal_position]);
			hinge_right_control->setTarget(hinge_pos_right[goal_position]);
		}
		else if (DOWN.changedToPressed() && goal_position > 0) {
			goal_position--;
			hinge_left_control->setTarget(hinge_pos_left[goal_position]);
			hinge_right_control->setTarget(hinge_pos_right[goal_position]);
		}

		// // Setting hinge position
		// printf("hinge position left  => %f\nhinge position right => %f\n\n", hinge_left.getPosition(), hinge_right.getPosition());

		pros::delay(10);
	}
}
