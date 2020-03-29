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
        // Timers
        orutil::CTimer m_blinkTimer;

        // Blink state variables
        static const int32_t BLINK_OFF = 0;
        static const int32_t BLINK_ON  = 1;

        bool    m_blinking;
        int32_t m_blink_state;
        int32_t m_blink_period;
};
