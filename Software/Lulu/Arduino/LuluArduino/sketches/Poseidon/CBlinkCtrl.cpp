#include "SysConfig.h"
#if(HAS_BLINK_CTRL)
#include "CBlinkCtrl.h"
#include "NCommManager.h"

const uint8_t OFF_PIN = 4;
const uint8_t SLOW_PIN = 3;
const uint8_t FAST_PIN = 2;
const uint8_t BUTTON_PUSHED = LOW;



CBlinkCtrl::CBlinkCtrl()
{
}

void CBlinkCtrl::Initialize()
{
	Serial.println( F( "BlinkCtrl_init:1;" ) );
	pinMode( OFF_PIN, INPUT_PULLUP);
	pinMode( SLOW_PIN, INPUT_PULLUP);
	pinMode( FAST_PIN, INPUT_PULLUP);

	m_statusTimer.Reset();
}

void CBlinkCtrl::Update( CCommand& commandIn )
{
	constexpr uint32_t kStatusDelay_ms	= 100;

	// Handle commands
	if( NCommManager::m_isCommandAvailable )
	{
		if( commandIn.Equals( "blinkCtrl:cmd" ) )
		{
			Serial.println( F( "blinkCtrl:cmd,1;" ) );
		}
	}

    	// Do other stuff 

    	// check for button pushes
	// only report latest button pushed, 
	if(digitalRead(OFF_PIN) == BUTTON_PUSHED){
		m_offButton = true;
		m_slowButton = false;
		m_fastButton = false;
	}
	if(digitalRead(SLOW_PIN) == BUTTON_PUSHED){
		m_offButton = false;
		m_slowButton = true;
		m_fastButton = false;
	}
	if(digitalRead(FAST_PIN) == BUTTON_PUSHED){
		m_offButton = false;
		m_slowButton = false;
		m_fastButton = true;
	}

	// every so often report latest button push and reset it
	if( m_statusTimer.HasElapsed( kStatusDelay_ms ) )
	{
    		String cmd = ""; 
		if(m_offButton){
			cmd = "blink(0);";
		} else if(m_fastButton){
			cmd = "blink(1,1500);";
		} else if(m_slowButton){
			cmd = "blink(1,150);";
	       	}

		// Report results
		if(cmd != ""){
			Serial.println(cmd);
		}
		m_offButton = false;
		m_slowButton = false;
		m_fastButton = false;
	}
}

#endif
