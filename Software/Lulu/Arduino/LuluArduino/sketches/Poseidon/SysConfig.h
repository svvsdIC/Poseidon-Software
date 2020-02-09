#pragma once


// TODO: The MCU ARCH and Board Type should eventually be passed in solely from the build script.

// ---------------------------------------------------------
// MCU Architecture Selection
// ---------------------------------------------------------

#ifndef ARDUINO_ARCH_AVR
	#error "This sketch is only supported on for ARDUINO_ARCH_AVR!"
#endif

// Configuration

// Mock
#define HAS_MOCK (1) 

// Some_module
#define HAS_SOME_MODULE (1)
