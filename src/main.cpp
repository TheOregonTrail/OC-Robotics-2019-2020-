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
 * If the robot is disabled or communications is lost, the autonomous task will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

  pros::Motor left_lift (1, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
  pros::Motor right_lift (10, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES); // This reverses the motor
  pros::Controller master (CONTROLLER_MASTER);
  pros::Motor left_front (20, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
  pros::Motor left_back (9, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
  pros::Motor right_front(11, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
  pros::Motor right_back (2, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
  pros::Motor claw (16, pros::E_MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);


void autonomous() {

	// Autonomous 


	left_front.move(-60) && left_back.move(-60) && right_front.move(-60) && right_back.move(-60);
	pros::delay(850);
	left_front.move(0) && left_back.move(0) && right_front.move(0) && right_back.move(0);
	left_lift.move(70) && right_lift.move(70);
	claw.move(40);
	pros::delay(250);
	left_lift.move(20) && right_lift.move(20);
	left_front.move(60) && left_back.move(60) && right_front.move(60) && right_back.move(60);
	pros::delay(2200);
	left_front.move(0) && left_back.move(0) && right_front.move(0) && right_back.move(0);
	claw.move(-80);
	pros::delay(400);
	left_front.move(-60) && left_back.move(-60) && right_front.move(-60) && right_back.move(-60);
	pros::delay(1500);
	left_front.move(-60) && left_back.move(-60) && right_front.move(60) && right_back.move(60);
	pros::delay(2100);
	left_front.move(0) && left_back.move(0) && right_front.move(0) && right_back.move(0);
	left_lift.move(40) && right_lift.move(40); // allocate more power to lift
	pros::delay(600);
	left_lift.move(20) && right_lift.move(20); // Lift dosn't lift high enough


	


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
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void oneStick() {
	//Motor Control
	if(abs(power) > 6 || abs(turn) > 6) {
	    left_front.move(power + turn) && left_back.move(power + turn);
	    right_front.move(power - turn) && right_back.move(power - turn);
    }else {
	    left_front.move(0) && left_back.move(0);
	    right_front.move(0) && right_back.move(0);

    }

	// Lift control
    left_lift.move(lift_analog);
    right_lift.move(lift_analog);

	//Claw Control
    if(master.get_digital(DIGITAL_R1)){
		claw.move(65);
		}
		else if(master.get_digital(DIGITAL_L1)){
		claw.move(-65);
		}
    else{
	    claw.move(0);
    }
}

void twoStick() {
	//Motor Control
	if(master.get_analog(ANALOG_LEFT_Y) > 6 || master.get_analog(ANALOG_RIGHT_Y) > 6) {
		left_front.move(master.get_analog(ANALOG_LEFT_Y)) && left_back.move(master.get_analog(ANALOG_LEFT_Y));
		right_front.move(master.get_analog(ANALOG_RIGHT_Y)) && right_back.move(master.get_analog(ANALOG_RIGHT_Y));
	}else{
		left_front.move(0) && left_back.move(0);
	    right_front.move(0) && right_back.move(0);
	}

	//Claw Control
	if(master.get_digital(DIGITAL_L2)){
		claw.move(65);
	}
	else if(master.get_digital(DIGITAL_L1)){
		claw.move(-65);
	}
    else{
	    claw.move(0);
    }

	//Lift Control
	if(master.get_digital(DIGITAL_R1)){
		left_lift.move(50);
		right_lift.move(50);
	}
	else if(master.get_digital(DIGITAL_R2)){
		left_lift.move(-50);
		right_lift.move(-50);
	}
    else{
	    left_lift.move(0);
		right_lift.move(0);
    }
}

bool stickMode = false;

void opcontrol() {
    // Hold the lifts in place
  left_lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  right_lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  double icp = claw.get_position(); // Initial Claw Position
  // test for a stream to the computer through the cable NOT WORKING
  std::cout << "Claw Initial Position: " << icp;

  // main running loop
  while (true) {

    int lift_analog = master.get_analog(ANALOG_RIGHT_Y);
    int power = master.get_analog(ANALOG_LEFT_Y);
    int turn = master.get_analog(ANALOG_LEFT_X);

    //Control mode
	if(stickMode){
		oneStick();
	}else{
		twoStick();
	}

    //Runs Autonomous Sequence
    if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B) &&
		master.get_digital(pros::E_CONTROLLER_DIGITAL_A) &&
		master.get_digital(pros::E_CONTROLLER_DIGITAL_X) &&
		master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))  {
        autonomous();
        continue;
    }
    pros::delay(20);
  }

}

