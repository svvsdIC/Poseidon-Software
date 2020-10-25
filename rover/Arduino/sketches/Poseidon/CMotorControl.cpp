#include "SysConfig.h"
#if(HAS_MOTOR_CONTROL)
#include "CMotorControl.h"
#include "NCommManager.h"
#include "joystick.h"

/*
Module responsible for driving the thrusters based on a diamond configuration

Serial commands in

	mtrCtrlVal(i, value); // i=which joystick, value -100 to 100 

*/

CMotorControl::CMotorControl()
{
}

void CMotorControl::Initialize()
{
	for (int i = 0; i < NUM_OUTPINS; i++)
	{
		motors[i].attach(outPins[i]);
	}
	
	Serial.println(F("mtrCtrlVal:1;"));
}

void CMotorControl::Update( CCommand& commandIn )
{
	// Handle commands
	if( NCommManager::m_isCommandAvailable )
	{
		if( commandIn.Equals( "mtrCtrlVal" ) )
		{
			//handle the arguments
		}
	}

    // Do other stuff 
}

#endif /* HAS_MOTOR_COONTROL */
