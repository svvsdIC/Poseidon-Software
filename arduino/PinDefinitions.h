#pragma once

#include <Arduino.h>
#include "SysConfig.h"

#if defined(BUILD_ROVER)
  #include "PinDefinitions_Rover.h"
#elif defined(BUILD_SURFACE)
  #include "PinDefinitions_Surface.h"
#else
  #error "HI PABLO PinDefinitions"
#endif
