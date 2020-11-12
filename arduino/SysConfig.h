#pragma once

#include "PinDefinitions.h"
#include "CompileOptions.h"

// TODO: The MCU ARCH and Board Type should eventually be passed in solely from the build script.

// ---------------------------------------------------------
// MCU Architecture Selection
// ---------------------------------------------------------

#ifndef ARDUINO_ARCH_AVR
	#error "This sketch is only supported on for ARDUINO_ARCH_AVR!"
#endif

// Configuration
#define HAS_OROV_CONTROLLERBOARD_25 (0)
#define HAS_STD_LIGHTS (0)
#define HAS_STD_CALIBRATIONLASERS (0)
#define HAS_CAMERASERVO (0)
#define HAS_STD_AUTOPILOT (0)
#define DEADMANSWITCH_ON (0)

#define HAS_MOCK (0)
#define HAS_SOME_MODULE (0)
#define HAS_BLINK (0)
#define HAS_TEMPERATURE_SENSOR (1)

// Selected Thruster Configuration
#define THRUSTER_CONFIGURATION THRUSTER_CONFIG_2X1
//#define THRUSTER_CONFIGURATION THRUSTER_CONFIG_2Xv2
//#define THRUSTER_CONFIGURATION THRUSTER_CONFIG_v2X1Xv2

// ---------------------------------------------------------
// After Market Modules
// ---------------------------------------------------------

// External Lights
#define HAS_EXT_LIGHTS (0)

// AltServo
#define HAS_ALT_SERVO (0)

// MS5803_14BA Depth Sensor
#define HAS_MS5803_14BA (0)

// MS5837_30BA Depth Sensor
#define HAS_MS5837_30BA (0)

// MPU9150
#define HAS_MPU9150 (0)

// BNO055 IMU
#define HAS_BNO055 (0)

// Receiver
#define HAS_RECEIVER (0)

//Some_module
#define HAS_SOME_MODULE (0)
