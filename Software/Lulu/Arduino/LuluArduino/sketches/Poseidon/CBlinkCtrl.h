#pragma once

// Includes
#include <orutil.h>
#include "CModule.h"

class CBlinkCtrl : public CModule
{
    public:
        CBlinkCtrl();
        void Initialize();
        void Update( CCommand& commandIn );
    private:
        // Timers
        orutil::CTimer m_statusTimer;
        int m_counter;
	uint8_t m_offButton = HIGH;
	uint8_t m_slowButton = HIGH;
	uint8_t m_fastButton = HIGH;
};
