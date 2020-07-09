
/******************************************************************************
TMP102_example.ino
Example for the TMP102 I2C Temperature Sensor
Alex Wende @ SparkFun Electronics
April 29th 2016
~

This sketch configures the TMP102 temperature sensor and prints the
temperature and alert state (both from the physical pin, as well as by
reading from the configuration register.

Resources:
Wire.h (included with Arduino IDE)
SparkFunTMP102.h

Development environment specifics:
Arduino 1.0+
Hardware Version 13

This code is beerware; if you see me (or any other SparkFun employee) at
the local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.   
******************************************************************************/

#include <Wire.h> 
#include "SparkFunTMP102.h" 
#include <SparkFunTSL2561.h>



TMP102 sensor0(0x48); 


SFE_TSL2561 light;
boolean gain;     
unsigned int ms;  


void setup() {
  Serial.begin(115200); 
  sensor0.begin(); 
  sensor0.setConversionRate(2);
  sensor0.setExtendedMode(0);
  light.begin();
  gain = 0;
  unsigned char time = 2;
  light.setTiming(gain,time,ms);
  light.setPowerUp();
}
 
void loop()
{
  float temperature;
  sensor0.wakeup();
  temperature = sensor0.readTempF();

  // Check for Alert
//  alertPinState = digitalRead(ALERT_PIN); // read the Alert from pin
  //alertRegisterState = sensor0.alert();   // read the Alert from register
  
  // Place sensor in sleep mode to save power.
  // Current consumtion typically <0.5uA.
  sensor0.sleep();

  // Print temperature and alarm state
  Serial.print("BCTR:");
  Serial.print(temperature);
  Serial.print(";");
  Serial.print("SC1I;");
  Serial.print(millis());
  Serial.print(";");
  Serial.print("SC2I;");
  Serial.print(random(0,250));
  Serial.print(";");
  Serial.print("SC3I;");
  Serial.print("2");
  Serial.print(";");
  Serial.print("BT2I:");
  Serial.print("3");
  Serial.print(";");
  Serial.print("BT1I:");
  Serial.print("4");
  Serial.print(";");
  Serial.print("BRDV:");
  Serial.print("5");
  Serial.print(";");
  Serial.print("vout:");
  Serial.print("6");
  Serial.print(";");
  Serial.print("iout:");
  Serial.print("7");
  Serial.print(";");
  Serial.print("time:");
  Serial.print("8");
  Serial.print(";");
  Serial.print("LUM");
  Serial.print(random());
  Serial.print(";");
  Serial.println("");
 
  
  //Serial.print("\tAlert Pin: ");
  //Serial.print(alertPinState);
  
  //Serial.print("\tAlert Register: ");
  //Serial.println(alertRegisterState);
  
  delay(1000);  // Wait 1000ms

  //Start Lux declarations

// Wait between measurements before retrieving the result
  // (You can also configure the sensor to issue an interrupt
  // when measurements are complete)
  
  // This sketch uses the TSL2561's built-in integration timer.
  // You can also perform your own manual integration timing
  // by setting "time" to 3 (manual) in setTiming(),
  // then performing a manualStart() and a manualStop() as in the below
  // commented statements:
  
  // ms = 1000;
  // light.manualStart();
  delay(ms);
  // light.manualStop();
  
  // Once integration is complete, we'll retrieve the data.
  
  // There are two light sensors on the device, one for visible light
  // and one for infrared. Both sensors are needed for lux calculations.
  
  // Retrieve the data from the device:

  unsigned int data0, data1;
  
  if (light.getData(data0,data1))
  {
    // getData() returned true, communication was successful
    
    Serial.print("data0: ");
    Serial.print(data0);
    Serial.print(" data1: ");
    Serial.print(data1);
  
    // To calculate lux, pass all your settings and readings
    // to the getLux() function.
    
    // The getLux() function will return 1 if the calculation
    // was successful, or 0 if one or both of the sensors was
    // saturated (too much light). If this happens, you can
    // reduce the integration time and/or gain.
    // For more information see the hookup guide at: https://learn.sparkfun.com/tutorials/getting-started-with-the-tsl2561-luminosity-sensor
  
    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    
    // Perform lux calculation:

    good = light.getLux(gain,ms,data0,data1,lux);
    
    // Print out the results:
  
    Serial.print(" lux: ");
    Serial.print(lux);
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
  }
  else
  {
    // getData() returned false because of an I2C error, inform the user.

    byte error = light.getError();
    printError(error);
  }

  //End Lux declarations
}

//Start Lux functions

void printError(byte error)
  // If there's an I2C error, this function will
  // print out an explanation.
{
  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  
  switch(error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}


//End lux functions
