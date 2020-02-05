#include "NDataManager.h"


//#include <I2C.h>

namespace NDataManager
{
	orutil::CTimer		m_timer_1hz;
	orutil::CTimer		m_timer_10hz;

	uint32_t			m_loopsPerSec = 0;

	// Called during Setup() to initialize any DataManager members to specific values
	void Initialize()
	{
        //m_thrusterData.MATC		= true;
	}

    int numLulus;
    void outputMockData (){
        Serial.print ("lulu:");Serial.print(++numLulus);Serial.println(";");
        if(numLulus >= 100){numLulus = 0;}
    };

	void HandleOutputLoops()
	{
		++m_loopsPerSec;

		// 1Hz update loop
		if( m_timer_1hz.HasElapsed( 1000 ) )
		{

			// Loops per sec
			Serial.print( F( "alps:" ) );
			Serial.print( m_loopsPerSec );
			Serial.println( ';' );

			// Reset loop counter
			m_loopsPerSec = 0;
		}

		// 10Hz Update loop
		if( m_timer_10hz.HasElapsed( 100 ) )
		{
			outputMockData();
		}
	}
}

