#pragma once

// Includes
#include <orutil.h>
#include <Servo.h>
#include "CModule.h"
#include "Common.h"

/*
TODO: 

*/
enum escPins {
    PORT_AFT
}



class CMotorControl : public CModule
{
    private:
        static const unsigned int NUM_INPINS = 4;
        static const unsigned int NUM_OUTPINS = 6;

        struct inputset { float value[NUM_INPINS]; };
        struct outputset { float value[NUM_OUTPINS]; };

        //frontLeft, frontRight, backLeft, backRight, vertical thruster A, vertical thruster B
        int outPins[NUM_OUTPINS] = {9, 6, 8, 7, 10, 11};
        //Individual motor calibration constants to allow for balancing if we have a slightly slower motor, etc...
        float motorConst[NUM_OUTPINS] = {0.8, 0.8, 0.8, 0.8, 1.0, 1.0};
        /* TODO: make motorConst configurable */
        //Zero out values in between +/- n% being sent to the motors (ie, don't ever give a motor 1.5% power (just give it 0%))
        static const float outCutoffConst = 4.0;
        
        //Input
        inputset controlValues;
        
        //Constants for the range of the ESC's input
        const int minOutMicros = 700;
        const int maxOutMicros = 2000;
        const int neutralValue = ((maxOutMicros - minOutMicros) / 2) + minOutMicros;//1350 is the middle, ie, 0% in [-100%, 100%]

        //Output
        outputset motorValues;
        // Other
        float controlPoints = 0.0; //global for easy debugging. See below for control point definition.

        // Servo object to control the motors (we like writeMicroseconds()...):
        Servo motors[NUM_OUTPINS];

        void updateMotors();
        outputset calcTransX();
        outputset calcTransY();
        outputset calcRotation();
        outputset calcVertical();
        void calcMotors();
        void parseCommand( CCommand & commandIn );

    public:
        CMotorControl();
        void Initialize();
        void Update( CCommand& commandIn );

};