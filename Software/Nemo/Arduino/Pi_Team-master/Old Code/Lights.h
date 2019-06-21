/*
  Lights.h - Test library for elights code
  Copyright (c) 2019 Caleb All.  All rights reserved.
*/

// ensure this library description is only included once
#ifndef Test_h
#define Test_h

// include types & constants of Wiring core API
#include "Arduino.h"

// library interface description
class Lights
{
  // user-accessible "public" interface
  public:
    Lights(string StringInput, string elightToken1, string elightToken2, elightToken3, int tokenLen);
    void elights(void);

  // library-accessible "private" interface
  private:
    int _ledBrightness;
    string _placeholder;
};

#endif

