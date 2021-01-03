#pragma once

#include <Arduino.h>
#include "SysConfig.h"

#ifdef BUILD_ROVER
  #include "PinDefinitions_Rover.h"
#elif BUILD_SURFACE
  #include "PinDefinitions_Surface.h"
#else
  #error "HI PABLO"
#endif
