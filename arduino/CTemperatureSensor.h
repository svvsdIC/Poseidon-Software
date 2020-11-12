#pragma once

// Includes
#include <orutil.h>
#include "CModule.h"

class CTemperatureSensor : public CModule
{
    public:
        CTemperatureSensor();
        void Initialize();
        void Update( CCommand& commandIn );
    private:
        // Timers
        orutil::CTimer m_statusTimer;
        int m_counter;
};
