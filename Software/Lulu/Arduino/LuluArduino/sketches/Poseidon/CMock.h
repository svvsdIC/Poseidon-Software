#pragma once

// Includes
#include <orutil.h>
#include "CModule.h"

class CMock : public CModule
{
    public:
        CMock();
        void Initialize();
        void Update( CCommand& commandIn );

    private:
        // Timers
        orutil::CTimer m_statusTimer;
        #define MOCK_STATUS_DELAY_MS 1000
        int m_counter;
        
        orutil::CTimer m_fastTimer;
        #define MOCK_FAST_DELAY_MS 100
        int m_fastCounter;
};
