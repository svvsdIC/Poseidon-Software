#include "SysConfig.h"
#if(HAS_BLINK)
#include "CBlink.h"
#include "NCommManager.h"

/*
Module responsible for blinking the builtin LED on the Arduino

Serial commands in

	blink:0			- turn off blinking
	blink:1,XXXX	- turn on blinking at XXX ms period

	// TODO:
	blink_status:
		reply with 0 or 1,XXXX
*/

CBlink::CBlink()
{
}

void CBlink::Initialize()
{
	// Set built in LED pin to output pinMode
	pinMode(LED_BUILTIN, OUTPUT);

	// Notify that blink module is ready
	Serial.println( F( "blink:1;" ) );
	m_statusTimer.Reset();	// TODO: do we need this?
}

void CBlink::Update( CCommand& commandIn )
{

	/* original code
	// TODO: remove delay functions
	digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(1000);                       // wait for a second
	digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
	delay(1000);
	*/

	// P1 ***** parse command if any ****

	// Handle commands (if there are any, otherwise this gets skipped)
	if( NCommManager::m_isCommandAvailable )
	{
		if( commandIn.Equals( "blink" ) )
		{
			int32_t blink_new_state = commandIn.m_arguments[1];

			// TODO: finish checking for valid values
			switch (blink_new_state) {
				case BLINK_OFF:
					

			}

			// TODO: set up standard error codes -- logging is good!!!
			// TODO: blink module implement error code or message?  initial implementation will iginore invalid input.
		}

		/*
		Sma
		*/
	}
/*  
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
 */

	// P2 ***********
	/*
		logic to implement a blink using timer values

		pseudo code:
			if blink is NOT enabled, then turn off the LED
			return

			if blink is enabled, then check timer agains period
				if expired, toggle LED
	*/
}

#endif /* HAS_BLINK */
