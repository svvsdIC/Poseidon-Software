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
        // Timers
        orutil::CTimer m_statusTimer;
        int m_counter;
        float value[NUM_INPINS];
        int minInMicros[NUM_INPINS], maxInMicros[NUM_INPINS];
        int inPins[NUM_INPINS] = {4, 5, 2, 3}; //LY, LX, RY, RX
        //Input
        inputset controlValues;
        //timer variables are used by the interrupt handler for reading PWM
        volatile unsigned long timerStart[NUM_INPINS];
        volatile unsigned long timeValue[NUM_INPINS];
        //RECIEVER INPUT ROUTINES
        /*
        * The interrupt#() functions are bound to any pin change events on the pins specified in the inPins array.
        * They all call the same readPWM() subroutine with their channel number (index number of their pin value
        * in the inPins array.
        */
        void interrupt0(){
        readPWM(0);
        }
        void interrupt1(){
        readPWM(1);
        }
        void interrupt2(){
        readPWM(2);
        }
        void interrupt3(){
        readPWM(3);
        }
        /*
        * readPWM() is called whenever a pin change event occurs. It has two behaviors, one for when the specific
        * pin goes HIGH, and one for when it goes LOW. The HIGH behavior simply records the current millis() for 
        * that channel. The LOW behavior subtracts the value recorded when the pin went HIGH from the current
        * millis() to obtain a duration of the pulse in milliseconds. Mapping and ranging operations are then
        * performed on the value to find the percent value for the control stick corresponding to the pin.
        */
        void readPWM(int channel)
        {
            unsigned long now = micros();
        
            if(digitalRead(inPins[channel]) == HIGH){
                timerStart[channel] = now;
            } 
            else if(digitalRead(inPins[channel]) == LOW){
                timeValue[channel] = now - timerStart[channel];//the pulse width
            }
        }
         //([whichPin], [handler routine], [listen for this]):
        PCintPort::attachInterrupt(inPins[0], interrupt0, CHANGE);
        PCintPort::attachInterrupt(inPins[1], interrupt1, CHANGE);
        PCintPort::attachInterrupt(inPins[2], interrupt2, CHANGE);
        PCintPort::attachInterrupt(inPins[3], interrupt3, CHANGE);

        for(int i = 0; i < NUM_INPINS; i++)
        {
            pinMode(inPins[i], INPUT);
        }
};
