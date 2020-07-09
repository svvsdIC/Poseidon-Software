
/******************************************************************************
TempandLux.ino
Example for the TMP102 I2C Temperature Sensor
Caleb All @ Innovation Center of St. Vrain Valley Schools
November 1st 2018
~

This sketch configures the TMP102 temperature sensor and prints the
temperature and alert state (both from the physical pin, as well as by
reading from the configuration register.

Resources:
Wire.h (included with Arduino IDE)
SparkFunTMP102.h
SparkFunTSL2561.h

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
double lux; 

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
  sensor0.sleep();

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
  Serial.print(lux);
  Serial.print(";");
  Serial.println("");
  
  unsigned int data0, data1;
  if (light.getData(data0,data1))
   {
     boolean good;
     good = light.getLux(gain,ms,data0,data1,lux);
   }

   delay(1000);
}
