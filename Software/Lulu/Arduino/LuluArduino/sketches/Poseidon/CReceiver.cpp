#include "SysConfig.h"
#if(HAS_RECEIVER)
#include <Arduino.h>
#include "CReceiver.h"
#include "NCommManager.h"
#include <orutil.h>


/*
Module responsible for reporting the Temperature from the TMP36 Temperature Sensor

Serial commands in

	We have no command input for this Module

Serial commands out

	Once a second report the Temperature in degrees Celsius as int * 1000
	"TemperatureSensor:XXXXXX;"

*/

CReceiver::CReceiver()
{
}

void CReceiver::Initialize()
{
	Serial.println( F( "TemperatureSensor_init:1;" ) );
	m_statusTimer.Reset();
}

void CReceiver::Update( CCommand& commandIn )
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
			// Read voltage
			float voltage = analogRead(A0) * 0.004882814;
			int32_t tempC = orutil::Encode1K((voltage - 0.5) * 100.0);

			// Report results
			Serial.print( F( "CReceiver:" ) );Serial.print(tempC); Serial.println(";");
		}
	}
}

#endif /* HAS_RECEIVER */
