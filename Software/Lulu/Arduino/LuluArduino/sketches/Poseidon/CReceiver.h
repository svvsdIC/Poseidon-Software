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
};
