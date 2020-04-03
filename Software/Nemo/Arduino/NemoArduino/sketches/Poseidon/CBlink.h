#pragma once

// Includes
#include <orutil.h>
#include "CModule.h"

class CBlink : public CModule
{
    public:
        CBlink();//constructor
        void Initialize();//init, needed
        void Update( CCommand& commandIn );//update, needed

    private:
        // Blink Timers variables
        orutil::CTimer m_blinkTimer;

        // Blink state variables
        static const int32_t BLINK_OFF = 0;
        static const int32_t BLINK_ON  = 1;

        bool m_is_blinking = false;
        int32_t m_blink_rate_ms = 1000;

        void parseCommand( CCommand & commandIn );
        void toggleLED();
};
