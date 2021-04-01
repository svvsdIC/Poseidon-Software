#pragma once

// Includes
#include <orutil.h>
#include "CModule.h"

class CReceiver : public CModule
{
    public:
        CReceiver();
        void Initialize();
        void Update( CCommand& commandIn );
    private:
        // other interesting variable
        long value[NUM_INPINS];
        int minInMicros[NUM_INPINS], maxInMicros[NUM_INPINS];//boundary variables for each channel      
        int inPins[NUM_INPINS] = {4, 5, 2, 3}; //LY receiver chan 4, LX receiver chan 3, RY receiver chan 2, RX receiver chan 1 

        long inCutoffConst = 9;
        //Zero out values in between +/- n% being sent to the motors (ie, don't ever give a motor 1.5% power (just give it 0%))
        //Same as above, but for input. (ie, don't ever accept an input value of 3% (just call it 0%)) Makes centering the joysticks easier.
        
        // Input
        inputset controlValues; // the percentage values that are sent to the pi
        
        // timer variables are used by the interrupt handler for reading PWM
        volatile unsigned long timerStart[NUM_INPINS];
        volatile unsigned long timeValue[NUM_INPINS];
};
