#pragma once

/*
CThruster_XXX modules are selectively compiled depending on
the value of the THRUSTER_CONFIGUTATION macro.  The value is
set in SysConfig_XXXXX.h.

The following define the values for each possible configuration.
When a new CThruster_XXX module is created, it will be necessary
to add a new definition here.
*/

#define THRUSTER_CONFIG_NONE    (0)
#define THRUSTER_CONFIG_2X1     (1)
#define THRUSTER_CONFIG_2Xv2    (2)
#define THRUSTER_CONFIG_v2X1Xv2 (3)
