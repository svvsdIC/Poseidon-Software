#pragma once

// Pin definitions
// ------------------------------
// See your core's variant.cpp for full pinmuxing configuration for this board
// ------------------------------

// ------------------------------
// Pin assignments
// ------------------------------

// Board LEDs
#define PIN_LED_0                   13
#define PIN_LED_1                   49

// Servo Pins (also digital outputs)
#define PIN_SERVO_1                 11
#define PIN_SERVO_2                 8
#define PIN_SERVO_3                 9
#define PIN_SERVO_4                 12
#define PIN_SERVO_5                 7
#define PIN_SERVO_6                 6
    
// PWM Pins
#define PIN_PWM_1                   44
#define PIN_PWM_2                   45
#define PIN_PWM_3                   46
#define PIN_PWM_4                   12

// GPIO
#define PIN_ENABLE_ESC              16
#define PIN_ENABLE_I2C          	48


// CMotor Controller pins
#define PIN_PORT_AFT                PIN_SERVO_4
#define PIN_PORT_FORWARD            PIN_SERVO_2
#define PIN_STARBOARD_AFT           PIN_SERVO_3
#define PIN_STARBOARD_FORWARD       PIN_SERVO_1
#define PIN_STARBOARD_VERTICAL      PIN_SERVO_5
#define PIN_PORT_VERTICAL           PIN_SERVO_6

// Other Servos
#define PIN_CAMERA_MOUNT		    PIN_SERVO_6
#define PIN_ALTSERVO			    PIN_SERVO_4

// -----------
// PWM pins

#define PIN_STANDARD_LIGHTS		    PIN_PWM_1
#define PIN_LASERS                  PIN_PWM_2
// Not yet used                     PIN_PWM_3
// Not yet used		    			PIN_PWM_4

// -----------
// Analog pins

#define PIN_BOARD_TEMP 		    	A8		// 
#define PIN_BOARD_I  		    	A0		// 
#define PIN_ESC1_I 			    	A3 		// 
#define PIN_ESC2_I 			    	A2 		// 
#define PIN_ESC3_I  		    	A1 		// 
#define PIN_BATT_TUBE1_I  	    	A6 		// 
#define PIN_BATT_TUBE2_I  	    	A5 		// 
#define PIN_BOARD_V 		    	A4 		//

// Other Defines
#define HAS_ESC_POWER_SWITCH		1
