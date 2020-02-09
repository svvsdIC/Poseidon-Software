#include "SysConfig.h"
#if(HAS_SOME_MODULE)
#include "CMock.h"
#include "NCommManager.h"


CMock::CMock()
{
}

void CMock::Initialize()
{
	Serial.println( F( "Some_module_init:1;" ) );
	m_statusTimer.Reset();
}

void CMock::Update( CCommand& commandIn )
{
	// Handle commands
	if( NCommManager::m_isCommandAvailable )
	{
		if( commandIn.Equals( "mock:cmd" ) )
		{
			Serial.println( F( "mock:cmd,1;" ) );
		}
	}

    // Do other stuff 
	if( m_fastTimer.HasElapsed( MOCK_STATUS_DELAY_MS ) )
	{
        // Report results
        Serial.print( F( "mock_cntr:" ) );Serial.println(++m_counter);

	}

	if( m_statusTimer.HasElapsed( MOCK_FAST_DELAY_MS ) )
	{
        // Report results
        Serial.print( F( "mock_fastCntr:" ) );Serial.println(++m_fastCounter);

        //reset counter every 100
        if(m_fastCounter > 99)
        {
            m_fastCounter = 0;
        }
	}
}

#endif
