/*
 The circuit:
 Rasp-Pi                  Arduino
---------------           ------------
x 3.3v    5v  x     +-----x Gnd
x GPIO 2  5v  x     |     x 13
X GPIO 3  Gnd x-----+     x 12 
X GPIO 4  Tx  x-----------x 11 (Rx)
X ...     Rx  x-----------x 10 (Tx)
x ...     ... x

 */

// prototypes
void    HandleOutputLoops();
void    UpdateControllerBoard();
double  GetTemp(void);
float   readVcc();
void    OutputDepthSensor();
void    OutputImuData();
void    OutputCbData();

// Includes
#include <SoftwareSerial.h>

// util stuff
#define NORMALIZE_ANGLE_360(a) ((a > 360.0f) ? (a - 360.0f) : ((a < 0.0f) ? (a + 360.0f) : a))
#define NORMALIZE_ANGLE_180(a) ((a > 180.0f) ? (a - 360.0f) : ((a < -180.0f) ? (a + 360.0f) : a))

namespace orutil
{
    class CTimer
    {
    private:
        // Last time the timer was successfully polled for an elapsed amount of time
        uint32_t m_lastTime_ms;    

    public:
        uint32_t Now();
        bool HasElapsed( uint32_t msIn );
        void Reset();
    };

    // CTimer 
    uint32_t CTimer::Now()
    {
        // Returns the current time in milliseconds
        return millis();
    }

    bool CTimer::HasElapsed( uint32_t msIn )
    {
        if( ( Now() - m_lastTime_ms ) > msIn )
        {
            // Update the last time to the current time
            m_lastTime_ms = Now();
            return true;
        }

        return false;
    }

    void CTimer::Reset()
    {
        m_lastTime_ms = Now();
    }

    // Float<->Int conversion helpers
    constexpr int32_t Encode1K( float valueIn )
    {
        return static_cast<int32_t>( valueIn * 1000.0f );
    }
    constexpr float Decode1K( int32_t valueIn )
    {
        return ( static_cast<float>( valueIn ) * 0.001f );
    }
} // end namespace orutil

// define structures
typedef struct cb_values_s{
    long    time;
    float   timeDelta     = 10;
    bool    brdvRampUp    = true;

    float sc1i;
    float sc2i;
    float sc3i;
    float brdi;    
    float bt1i;    
    float bt2i;    
    float brdv;
    float vout;
    float iout;    
    float avcc;    
};
cb_values_s cb;

typedef struct imu_values_s {
    
    long time         = 0;
    long timeDelta_ms = 1000;

    bool    mode;
    float   roll;
    float   pitch;
    float   pitchOffset;
    float   yaw;
    float   yawOffset;
    float   heading;
};
imu_values_s imu;

typedef struct depthSensor_s{
    long    time         = 0;
    long    timeDelta_ms = 50; 
    bool    waterType;
    float   depth;
    float   depthOffset;
    float   temperature;
    float   pressure;
};
depthSensor_s depthSensor;

// global items
  long  startMillis;
  SoftwareSerial    SerialToPi(11,10); //11-Rx, 10-Tx
  orutil::CTimer    m_timer_1sec;
  orutil::CTimer    m_timer_100ms;
  orutil::CTimer    m_timer_3sec;


void setup()
{
    startMillis = millis();

    
    SerialToPi.begin(115200);
    Serial.begin(115200);
    delay(500);
	  SerialToPi.println( F( "boot:1;" ) );

}

void loop()
{
  OutputSensorData();
	// Attempt to read a current command off of the command line
//	NCommManager::GetCurrentCommand();

	// Handle any config change requests
//	NVehicleManager::HandleMessages( NCommManager::m_currentCommand );

	// Handle update loops for each module
//	NModuleManager::HandleModuleUpdates( NCommManager::m_currentCommand );


	// Handle update loops that send data back to the beaglebone
//	NDataManager::HandleOutputLoops();
        //HandleOutputLoops();
}

long m_loopsPerSec = 0;

void HandleOutputLoops(){
    ++m_loopsPerSec;

    // 1Hz update loop
    if( m_timer_1sec.HasElapsed( 1000 ) )
    {
      // Send shared data to beaglebone
      OutputCbData();

      // Loops per sec
      SerialToPi.print( F( "alps:" ) );
      SerialToPi.print( m_loopsPerSec );
      SerialToPi.print( ";" );
      SerialToPi.println();

      // Reset loop counter
      m_loopsPerSec = 0;
    }

    // 10Hz Update loop
    if( m_timer_100ms.HasElapsed( 100 ) )
    {
      OutputImuData();
    }

    if( m_timer_3sec.HasElapsed( 3000 ) )
    {
      OutputDepthSensor();
    } 
}

//------------------------------------------------------
void  OutputDepthSensor(){
    // Update time
    depthSensor.time += depthSensor.timeDelta_ms;

    // Generate depth from -10:10 meters
    depthSensor.depth = 10 * sin( depthSensor.time * ( 3.14159 / 20000 ) );

    // Generate temperature from 15:25 degrees
    depthSensor.temperature = 20 + ( 5 * sin( depthSensor.time * ( 3.14159 / 40000 ) ) );

    // Generate pressure from 50:70 kPa
    depthSensor.pressure = 60 + ( 10 * sin( depthSensor.time * ( 3.14159 / 40000 ) ) );

    // Create result string (Note: we don"t bother to take into account water type or offsets w.r.t. temperature or pressure )
    
    SerialToPi.print( "depth_d:" ); 
    SerialToPi.print(orutil::Encode1K( depthSensor.depth - depthSensor.depthOffset ));
    SerialToPi.print(";");
    SerialToPi.print( "depth_t:" );
    SerialToPi.print(orutil::Encode1K( depthSensor.temperature ));
    SerialToPi.print(";");
    SerialToPi.print( "depth_p:" );
    SerialToPi.print(orutil::Encode1K( depthSensor.pressure ));
    SerialToPi.print(";");
    SerialToPi.println();
}

//------------------------------------------------------
void  OutputImuData(){
    // Update time
    imu.time += imu.timeDelta_ms;

    // Generate pitch -90:90 degrees
    imu.pitch = 90 * sin( imu.time * ( 3.14159 / 10000 ) );
    
    // Generate roll -90:90 degrees
    imu.roll = 90 * sin( imu.time * ( 3.14159 / 30000 ) );
    
    // Generate yaw between -120:120 degrees
    long baseYaw = 120 * sin( imu.time * ( 3.14159 / 10000 ) );

    // Handle mode switches (gyro mode is -180:180, mag mode is 0:360)
    if( imu.mode == 0 )
    {
      imu.yaw = baseYaw;
    }
    else if( imu.mode == 1 )
    {
      imu.yaw = NORMALIZE_ANGLE_360( baseYaw );
    }
    SerialToPi.print( F( "imu_r:" ) );  SerialToPi.print( orutil::Encode1K( imu.roll ) );  SerialToPi.print( ";" );
    SerialToPi.print( F( "imu_p:" ) );  SerialToPi.print( orutil::Encode1K( imu.pitch ) ); SerialToPi.print( ";" );
    SerialToPi.print( F( "imu_y:" ) );  SerialToPi.print( orutil::Encode1K( imu.yaw ) );   SerialToPi.print( ";" );
    SerialToPi.println();
}
//Start of the sensors (mock at the moment)



void OutputSensorData(void) {
  imu.time += imu.timeDelta_ms;
  float val = imu.time;
  String sensorresult = "";
  
  sensorresult += "ORP:" + String(round(1020 * sin(val * (PI / 10000)))) + ";";
  sensorresult += "conductivity:" + String(round(abs(500000 * sin(val * (PI /20000))))) + ";";
  sensorresult += "disOxy:" + String(round(abs(100 * sin(val * PI / 20000)))) + ";";
  sensorresult += "pH:" + String(round(abs(14 * sin(val * (PI / 10000))))) + ";";
  sensorresult += "luminosity:" + String(round(abs(65535 * sin(val * (PI / 10000))) / 10)) + ";";
  sensorresult += "waterTemp:" + String(round(abs(100 * sin(val * (PI / 20000))) * 10) / 10) + ";";
  Serial.println(sensorresult);
  delay(1000);
}


//End of the sensors

//------------------------------------------------------
void    OutputCbData(void){
    // Update time
    cb.time += 100;

    // Generate a current baseline from 1:2 amps
    float currentBase = ( ( random(0,100) / 100.0 ) + 1.0 );

    // Generate currents for each battery tube from the base current, deviation of +/- 0.2A
    cb.bt1i = currentBase + ( ( random(400) / 1000.0 ) - 0.2 );
    cb.bt2i = currentBase + ( ( random(400) / 1000.0 ) - 0.2 );
    cb.sc1i = currentBase + ( ( random(1000) / 100.0 )  );
    cb.sc2i = currentBase + ( ( random(400) / 1000.0 )  );
    cb.sc3i = currentBase + ( ( random(400) / 1000.0 )  );
    cb.iout = currentBase + ( ( random(1200) / 1000.0 )  );
    
    // Get total current by adding the two tube currents
    cb.brdi = cb.bt1i + cb.bt2i;

    // Generate board voltage (ramps up and down between 5V and 12V)
    if( cb.brdvRampUp )
    {
      cb.brdv += 0.5;
      if( cb.brdv >= 12 ) { 
        cb.brdvRampUp = false; }
    }
    else
    {
      cb.brdv -= 0.5;
      if( cb.brdv <= 5 )
      {
        cb.brdvRampUp = true;
      }
    }
    
    cb.vout = readVcc();

    SerialToPi.print("BRDT:");SerialToPi.print(GetTemp());SerialToPi.print(";");
    SerialToPi.print("SC1I;");SerialToPi.print(cb.sc1i);SerialToPi.print(";");
    SerialToPi.print("SC2I;");SerialToPi.print(cb.sc3i);SerialToPi.print(";");
    SerialToPi.print("SC3I;");SerialToPi.print(cb.sc3i);SerialToPi.print(";");
    SerialToPi.print("BT2I:");SerialToPi.print(cb.bt2i);SerialToPi.print(";");
    SerialToPi.print("BT1I:");SerialToPi.print(cb.bt1i);SerialToPi.print(";");
    SerialToPi.print("BRDV:");SerialToPi.print(cb.brdv);SerialToPi.print(";");
    SerialToPi.print("vout:");SerialToPi.print(cb.vout);SerialToPi.print(";");
    SerialToPi.print("iout:");SerialToPi.print(cb.iout);SerialToPi.print(";");
    SerialToPi.print("time:");SerialToPi.print(millis()-startMillis);SerialToPi.print(";");
    SerialToPi.println();
}

//----------------------------------------------------
float mapf( long x, long in_min, long in_max, long out_min, long out_max ){
    return ( float )( x - in_min ) * ( out_max - out_min ) / ( float )( in_max - in_min ) + out_min;
}

//----------------------------------------------------
double GetTemp(void){
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 324.31 ) / 1.22;

  // The returned temperature is in degrees Celsius.
  return (t);
}

//------------------------------------------------------
float readVcc() { 
  #define VBATPIN A0  //wired to 3.3V ref
    
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 5.0;  // Multiply by 5.0V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  return measuredvbat;  // adalogger has no access to battery Vcc
}

//------------------------------------------------------
