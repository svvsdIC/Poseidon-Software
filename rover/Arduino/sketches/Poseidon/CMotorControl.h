#pragma once

// Includes
#include <orutil.h>
#include <Servo.h>
#include "CModule.h"

class CMotorControl : public CModule
{
    private:
        static const int NUM_OUTPINS = 6;
        //frontLeft, frontRight, backLeft, backRight, vertical thruster A, vertical thruster B
        static const int outPins[NUM_OUTPINS] = {9, 6, 8, 7, 10, 11};
        
                //rvo object to control the motors (we like writeMicroseconds()...):
        Servo motors[NUM_OUTPINS];

    public:
        CMotorControl();
        void Initialize();
        void Update( CCommand& commandIn );

};