#include "SysConfig.h"
#if(HAS_BLINK)
#include "CBlink.h"
#include "NCommManager.h"


CBlink::CBlink()
{
}

void CBlink::Initialize()
{
	Serial.println( F( "blink:1;" ) );
	m_statusTimer.Reset();
}

void CBlink::Update( CCommand& commandIn )
{
	// Handle commands
	if( NCommManager::m_isCommandAvailable )
	{
		if( commandIn.Equals( "blink:cmd" ) )
		{
			Serial.println( F( "blink:cmd,1;" ) );
		}
	}

    // Do other stuff 
	if( m_fastTimer.HasElapsed( BLINK_STATUS_DELAY_MS ) )
	{
        // Report results
        Serial.print( F( "blink_cntr:" ) );Serial.println(++m_counter);

	}

	if( m_statusTimer.HasElapsed( BLINK_FAST_DELAY_MS ) )
	{
        // Report results
        Serial.print( F( "blink_fastCntr:" ) );Serial.println(++m_fastCounter);

        //reset counter every 100
        if(m_fastCounter > 99)
        {
            m_fastCounter = 0;
        }
	}
}

#endif
