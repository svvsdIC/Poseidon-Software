#include "SysConfig.h"
#if(HAS_SOME_MODULE)
#include "CSome_module.h"
#include "NCommManager.h"


CSome_module::CSome_module()
{
}

void CSome_module::Initialize()
{
	Serial.println( F( "Some_module_init:1;" ) );
	m_statusTimer.Reset();
}

void CSome_module::Update( CCommand& commandIn )
{
	constexpr uint32_t kStatusDelay_ms	= 1000;							// 1hz

    
	// Handle commands
	if( NCommManager::m_isCommandAvailable )
	{
		if( commandIn.Equals( "some_mod:cmd" ) )
		{
			Serial.println( F( "some_mod:cmd,1;" ) );
		}
	}

    // Do other stuff 
	if( m_statusTimer.HasElapsed( kStatusDelay_ms ) )
	{
		{
			// Report results
			Serial.print( F( "some_mod_cntr:" ) );Serial.println(++m_counter);
		}
	}
}

#endif
