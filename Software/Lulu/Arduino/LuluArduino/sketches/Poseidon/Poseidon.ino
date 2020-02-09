// Includes
//#include <EEPROM.h>

#include "NArduinoManager.h"
#include "NCommManager.h"
#include "NModuleManager.h"
#include <orutil.h>

void setup()
{
	// Initialize main subsystems
	NArduinoManager::Initialize();
	NCommManager::Initialize();
	NModuleManager::Initialize();


	// Boot complete
	Serial.println( F( "boot:1;" ) );
}


uint32_t			m_loopsPerSec = 0;
orutil::CTimer      m_timer_1hz;

void loop()

{
	// Reset the watchdog timer
	wdt_reset();

	// Attempt to read a current command off of the command line
	NCommManager::GetCurrentCommand();

	// Handle update loops for each module
	NModuleManager::HandleModuleUpdates( NCommManager::m_currentCommand );

    ++m_loopsPerSec;

    // 1Hz update loop
    if( m_timer_1hz.HasElapsed( 1000 ) )
    {

        // Loops per sec
        Serial.print( F( "lulu_lps:" ) );
        Serial.print( m_loopsPerSec );
        Serial.println( ';' );

        // Reset loop counter
        m_loopsPerSec = 0;
    }

}
