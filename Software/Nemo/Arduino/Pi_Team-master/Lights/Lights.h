/*
  Lights.h - Test library for elights code
  Copyright (c) 2019 Caleb All.  All rights reserved.
*/

#ifndef Lights_h
#define Lights_h

#include "Arduino.h"

class Lights
{
  // user-accessible "public" interface
  public:
    Lights(String elightToken1, String elightToken2, String elightToken3, int tokenLen, int value_1, int value_2, int value_3);
    void elights(String stringInput);

  // library-accessible "private" interface
  private:
    int _ledBrightness;
    String _placeholder;
    String _elightToken1;
    String _elightToken2;
    String _elightToken3;
    int _tokenLen;
};

#endif
