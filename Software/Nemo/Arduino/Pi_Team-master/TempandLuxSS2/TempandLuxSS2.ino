
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
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

TMP102 sensor0(0x48); 

SFE_TSL2561 light;

boolean gain;     
unsigned int ms;
double lux; 

void setup() {
  mySerial.begin(115200); 
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

if (mySerial.available() > 0) {
       mySerial.print("BCTR:");
       mySerial.print(temperature);
       mySerial.print(";");
       mySerial.print("SC1I;");
       mySerial.print(millis());
       mySerial.print(";");
       mySerial.print("SC2I;");
       mySerial.print(random(0,250));
       mySerial.print(";");
       mySerial.print("SC3I;");
       mySerial.print("2");
       mySerial.print(";");
       mySerial.print("BT2I:");
       mySerial.print("3");
       mySerial.print(";");
       mySerial.print("BT1I:");
       mySerial.print("4");
       mySerial.print(";");
       mySerial.print("BRDV:");
       mySerial.print("5");
       mySerial.print(";");
       mySerial.print("vout:");
       mySerial.print("6");
       mySerial.print(";");
       mySerial.print("iout:");
       mySerial.print("7");
       mySerial.print(";");
       mySerial.print("time:");
       mySerial.print("8");
       mySerial.print(";");
       mySerial.print("LUM");
       mySerial.print(lux);
       mySerial.print(";");
       mySerial.println("");
  
       unsigned int data0, data1;
       if (light.getData(data0,data1))
       {
         boolean good;
         good = light.getLux(gain,ms,data0,data1,lux);
       }
    }


   delay(1000);
}
