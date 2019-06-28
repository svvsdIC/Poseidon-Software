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
  _elightToken1 = elightToken1;
  _elightToken2 = elightToken2;
  _elightToken3 = elightToken3;
  _value_1 = value_1;
  _value_2 = value_2;
  _value_3 = value_3;
  _tokenLen = tokenLen;

}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void Lights::elights(String stringInput)
{
  if (stringInput.startsWith(_elightToken1)) {
    _placeholder = stringInput.substring((_tokenLen + 1)); //Account for separator
    _ledBrightness = _placeholder.toInt();
    analogWrite(_value_1, _ledBrightness);
    }

if (stringInput.startsWith(_elightToken2)) {
    _placeholder = stringInput.substring((_tokenLen + 1)); //Account for separator
    _ledBrightness = _placeholder.toInt();
    analogWrite(_value_2, _ledBrightness);
    }

if (stringInput.startsWith(_elightToken3)) {
    _placeholder = stringInput.substring((_tokenLen + 1)); //Account for separator
    _ledBrightness = _placeholder.toInt();
    analogWrite(_value_3, _ledBrightness);
    }
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library
