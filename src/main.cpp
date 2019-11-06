#include "robotconf.h"
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
        pros::lcd::clear_line(1);
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

    // This string can be maximum 32 chars long
	pros::lcd::set_text(1, "           Autonomous        ");
    // pros::lcd::set_text(2, "Blue");



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
void competition_initialize() {

}


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

 
void autonomous() {

	// Autonomous 


	left_lift.move(35) && right_lift.move(35);
	pros::delay(800);
	left_lift.move(15) && right_lift.move(15);
	left_front.move(-60) && left_back.move(-60) && right_front.move(-60) && right_back.move(-60);
	pros::delay(1250);
	left_lift.move(20) && right_lift.move(20);
	left_front.move(0) && left_back.move(0) && right_front.move(0) && right_back.move(0);
	claw.move(40);
	pros::delay(250);
	left_lift.move(20) && right_lift.move(20);
	left_front.move(100) && left_back.move(80) && right_front.move(80) && right_back.move(80);
	pros::delay(1000);
	left_front.move(0) && left_back.move(0) && right_front.move(0) && right_back.move(1);




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

// int l_motor_speed = 0; For Future Abstraction "Chalie"
// int r_motor_speed = 0; ^^^


// Driver control setting for one stick driving
void oneStick() {

    int lift_analog = master.get_analog(ANALOG_RIGHT_Y);
    int power = master.get_analog(ANALOG_LEFT_Y);
    int turn = master.get_analog(ANALOG_LEFT_X);



    // Allows for drive train control over the one stick
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

	/*Drive train control with two sticks
     * The abs() function is used for deadzones on the controller
     * get_analog() retrieves the value from the controllers joystick
     */

	if(abs(master.get_analog(ANALOG_LEFT_Y)) > 6 || abs(master.get_analog(ANALOG_RIGHT_Y)) > 6) {
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
		left_lift.move(90);
		right_lift.move(90);
	}
	else if(master.get_digital(DIGITAL_R2)){
		left_lift.move(-20);
		right_lift.move(-20);
	}
    else{
	    left_lift.move(10);
		right_lift.move(10);
    }
}

bool stickMode = true;

void opcontrol() {
    // Hold the lifts in place
  left_lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  right_lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  double icp = claw.get_position(); // Initial Claw Position
  // test for a stream to the computer through the cable NOT WORKING
  std::cout << "Claw Initial Position: " << icp;

  // main running loop
  while (true) {

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

