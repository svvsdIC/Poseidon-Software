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

Lights::elights(int givenValue)
{
  // initialize this instance's variables
  value = givenValue;

  // do whatever is required to initialize the library
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void Test::doSomething(void)
{
  // eventhough this function is public, it can access
  // and modify this library's private variables
  Serial.print("value is ");
  Serial.println(value);

  // it can also call private functions of this library
  doSomethingSecret();
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

void Test::doSomethingSecret(void)
{
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
}

