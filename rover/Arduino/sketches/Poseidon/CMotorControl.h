#pragma once

// Includes
#include <orutil.h>
#include <Servo.h>
#include "CModule.h"

/*
TODO: 
    * Give better variable names for RX,RY,LX,LY
    * Consider using an enum
*/



class CMotorControl : public CModule
{
    private:
        struct outputset { float value[NUM_OUTPINS]; };
        struct inputset { float value[NUM_INPINS]; };

        static const int NUM_INPINS = 4;
        static const int NUM_OUTPINS = 6;
        //frontLeft, frontRight, backLeft, backRight, vertical thruster A, vertical thruster B
        static const int outPins[NUM_OUTPINS] = {9, 6, 8, 7, 10, 11};
        //Individual motor calibration constants to allow for balancing if we have a slightly slower motor, etc...
        static const float motorConst[NUM_OUTPINS] = {0.8, 0.8, 0.8, 0.8, 1.0, 1.0};
        /* TODO: make motorConst configurable */
        //Zero out values in between +/- n% being sent to the motors (ie, don't ever give a motor 1.5% power (just give it 0%))
        static const float outCutoffConst = 4.0;
        
        //Input
        inputset controlValues;
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