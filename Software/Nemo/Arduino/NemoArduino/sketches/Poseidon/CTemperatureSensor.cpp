#include "SysConfig.h"
#if(HAS_SOME_MODULE)
#include <Arduino.h>
#include "CTemperatureSensor.h"
#include "NCommManager.h"


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
	if( NCommManager::m_isCommandAvailable )
	{
		if( commandIn.Equals( "TemperatureSensor:cmd" ) )
		{
			Serial.println( F( "TemperatureSensor:cmd,1;" ) );
		}
	}

    // Do other stuff
	if( m_statusTimer.HasElapsed( kStatusDelay_ms ) )
	{
		{
			// Report results
			Serial.print( F( "TemperatureSensor_cntr:" ) );Serial.println(++m_counter);
		}
	}
}

#endif
