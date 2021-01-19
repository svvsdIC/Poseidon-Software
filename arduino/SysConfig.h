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
#if defined(BUILD_ROVER)
  #include "SysConfig_Rover.h"
#elif defined(BUILD_SURFACE)
  #include "SysConfig_Surface.h"
#else
  #include "BuildErrorMessage.h"
#endif
