#include "SysConfig.h"
#if(HAS_BLINK)
#include "CBlink.h"
#include "NCommManager.h"

/*
Module responsible for blinking the builtin LED on the Arduino

Serial commands in

	blink(0);		- turn off blinking
	blink(1,XXXX);	- turn on blinking at XXX ms period

	// TODO:
	blink_status:
		reply with 0 or 1,XXXX
*/

// TODO: set up standard error codes -- logging is good!!!
// TODO: blink module implement error code or message?  initial implementation will iginore invalid input.


CBlink::CBlink()
{
}

void CBlink::Initialize()
{
	// Set built in LED pin to output pinMode
	pinMode(LED_BUILTIN, OUTPUT);

	// Notify that blink module is ready
	Serial.println( F( "blink:1;" ) );		// TODO: revist how we notify the module is running
	m_blinkTimer.Reset();
}

void CBlink::Update( CCommand& commandIn )
{
	if( NCommManager::m_isCommandAvailable && commandIn.Equals( "blink" ))
	{
		parseCommand(commandIn);
	}

	toggleLED();
}

void CBlink::parseCommand( CCommand &commandIn) {
	// Handle commands (if there are any, otherwise this gets skipped)
	Serial.println(commandIn.m_text);
	for (int i=0; i <= commandIn.m_arguments[0]; i++){
		Serial.println(commandIn.m_arguments[i]);
	}
	if(commandIn.m_arguments[0] == 0){
		Serial.println("blink:no_args;"); //error message
		return;
	}
	if(commandIn.m_arguments[0] > 2){
		Serial.println("blink:too_many_args;"); //error message
		return;
	}
	int32_t blink_new_state = commandIn.m_arguments[1];
	// TODO: finish checking for valid values
	switch (blink_new_state) {
		case BLINK_OFF:
		//turn off blinking
			digitalWrite(LED_BUILTIN, LOW); 
			m_is_blinking = false;
			break;

		case BLINK_ON:
		//turn on blinking at a set rate
			if(commandIn.m_arguments[0] == 1){
				m_blink_rate_ms = DEFAULT_BLINK_RATE;	
			}
			else{
				m_blink_rate_ms = commandIn.m_arguments[2];	
			}
			m_is_blinking = true;
			break;
			
		default:
			Serial.println("blink:bad_command;"); //error message
			break;
	}
}

void CBlink::toggleLED() {
	// Toggle the LED when the timer has elapsed
	if(m_is_blinking && m_blinkTimer.HasElapsed(m_blink_rate_ms)){
			bool current_led_state = digitalRead(LED_BUILTIN);
			digitalWrite(LED_BUILTIN, !current_led_state);
		}
}

#endif /* HAS_BLINK */

// - --- original code - keep for reference -----

/* original code
	// TODO: remove delay functions
	digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(1000);                       // wait for a second
	digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
	delay(1000);
	*/

	/*
		logic to implement a blink using timer values

		pseudo code:
			if blink is NOT enabled, then turn off the LED
			return

			if blink is enabled, then check timer agains period
				if expired, toggle LED
	*/


/*  OLD CODE - keep for reference, then delete

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