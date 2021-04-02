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
	Serial.println( F( "CReceiver_init:1;" ) );
	m_statusTimer.Reset();
	
	// Configure the hardware pins as inputs
	for(int i = 0; i < NUM_INPINS; i++)
	{
		pinMode(inPins[i], INPUT);
	}

    // attach pins to interrupt routines to catch level changes
	//([whichPin], [handler routine], [listen for this]):
	PCintPort::attachInterrupt(inPins[TRANSX], interruptTRANSX, CHANGE);
	PCintPort::attachInterrupt(inPins[TRANSY], interruptTRANSY, CHANGE);
	PCintPort::attachInterrupt(inPins[TRANSZ], interruptTRANSZ, CHANGE);
	PCintPort::attachInterrupt(inPins[YAW], interruptYAW, CHANGE);
}

void CReceiver::Update( CCommand& commandIn )
{
	// Handle commands
	/*
	if( NCommManager::m_isCommandAvailable )
	{
		if( commandIn.Equals( "CReceiver:cmd" ) )
		{
			Serial.println( F( "CReceiver:cmd,1;" ) );
		}
	}
	*/

	static bool update_ready = false;
    // Check if there are new values from the interrupt routines
	for(unsigned int i=0; i<NUM_INPINS; ++i)
	{
		// None Zero values means that the PWM measurement is ready for that channel
		if(timeValue[i] != 0 && timeValue[i] > 700 && timeValue[i] < 2100)
		{
			// Calculate control values and send them
			controlValues.value[i] = map(timeValue[i], minInMicros[i], maxInMicros[i], -100, 100);	// TODO: may want to revist rage.  Lost resolution
			if(abs(controlValues.value[i]) < inCutoffConst)
			{
				//zero out values less than the allowed constant right here (to make it easier to "center" the sticks)
				controlValues.value[i] = 0;
			}

			// Prepare to read the next time value for this channel
			timeValue[i]=0;
			update_ready = true;
		}
	}



	// Send Control value
	// mtrctl:1,2,3,4;
	if (update_ready) {
		Serial.print( F( "mtrctl:" ) );
		Serial.print(controlValues.value[0]);
		for(unsigned int i=1; i<NUM_INPINS; ++i)
		{
			Serial.print( ",");
			Serial.print(controlValues.value[i]);
		}
		Serial.println(";");
		update_ready = false;
	}
}
	

//RECEIVER INPUT ROUTINES
/*
* The interrupt#() functions are bound to any pin change events on the pins specified in the inPins array.
* They all call the same readPWM() subroutine with their channel number (index number of their pin value
* in the inPins array.
*/
void interruptTRANSX(){
	readPWM(TRANSX);
}
void interruptTRANSY(){
	readPWM(TRANSY);
}
void interruptTRANSZ(){
	readPWM(TRANSZ);
}
void interruptYAW(){
	readPWM(TRANSY);
}

/*
* readPWM() is called whenever a pin change event occurs. It has two behaviors, one for when the specific
* pin goes HIGH, and one for when it goes LOW. The HIGH behavior simply records the current millis() for 
* that channel. The LOW behavior subtracts the value recorded when the pin went HIGH from the current
* millis() to obtain a duration of the pulse in milliseconds. Mapping and ranging operations are then
* performed on the value to find the percent value for the control stick corresponding to the pin.
*/
void readPWM(int channel)
{
	unsigned long now = micros();

	if(digitalRead(inPins[channel]) == HIGH){
		timerStart[channel] = now;
	} 
	else if(digitalRead(inPins[channel]) == LOW){
		timeValue[channel] = now - timerStart[channel];//the pulse width
	}
}

#endif /* HAS_RECEIVER */
