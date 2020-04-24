#include "SysConfig.h"
#if(HAS_TEMPERATURE_SENSOR)
#include <Arduino.h>
#include "CTemperatureSensor.h"
#include "NCommManager.h"


/*
Module responsible for reporting the Temperature from the TMP36 Temperature Sensor

Serial commands in

	We have no command input for this Module

Serial commands out

	Once a second report the Temperature in Celius
	"TemperatureSensor:XXX;"

*/

CTemperatureSensor::CTemperatureSensor()
{
}

void CTemperatureSensor::Initialize()
{
	Serial.println( F( "TemperatureSensor_init:1;" ) );
	m_statusTimer.Reset();
}

void CTemperatureSensor::Update( CCommand& commandIn )
{
	constexpr uint32_t kStatusDelay_ms	= 1000;							// 1hz


	// Handle commands
/*
	if( NCommManager::m_isCommandAvailable )
	{
		if( commandIn.Equals( "TemperatureSensor:cmd" ) )
		{
			Serial.println( F( "TemperatureSensor:cmd,1;" ) );
		}
	}
*/

    // Do other stuff
	if( m_statusTimer.HasElapsed( kStatusDelay_ms ) )
	{
		{
			// Report results
			Serial.print( F( "TemperatureSensor_cntr:" ) );Serial.println(++m_counter);
		}
	}
}

#endif /* HAS_TEMPERATURE_SENSOR */
