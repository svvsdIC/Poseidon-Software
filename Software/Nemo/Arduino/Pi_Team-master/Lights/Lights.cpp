/*
  Lights.cpp - Test library for elights
  Copyright (c) 2019 Caleb All.  All rights reserved.
*/

// include core Wiring API
#include "Arduino.h"

// include this library's description file
#include "Lights.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

Lights::Lights(String elightToken1, String elightToken2, String elightToken3, int tokenLen, int value_1, int value_2, int value_3)
{

  // do whatever is required to initialize the library
  pinMode(value_1, OUTPUT);
  pinMode(value_2, OUTPUT);
  pinMode(value_3, OUTPUT);
  Serial.begin(115200);

}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void Lights::elights(String stringInput)
{
  if (stringInput.startsWith(elightToken1)) {
    placeholder = stringInput.substring((tokenLen + 1)); //Account for separator
    _ledBrightness = _placeholder.toInt();
    analogWrite(value_1, ledBrightness);
    }

if (stringInput.startsWith(elightToken2)) {
    placeholder = stringInput.substring((tokenLen + 1)); //Account for separator
    _ledBrightness = _placeholder.toInt();
    analogWrite(value_2, ledBrightness);
    }

if (stringInput.startsWith(elightToken3)) {
    placeholder = stringInput.substring((tokenLen + 1)); //Account for separator
    _ledBrightness = _placeholder.toInt();
    analogWrite(value_3, ledBrightness);
    }

else {
  Serial.print("Error: Unrecognized Input")
}
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library
