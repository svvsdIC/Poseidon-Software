#pragma once

// Includes
#include <orutil.h>
#include "CModule.h"

class CSome_module : public CModule
{
    public:
        CSome_module();
        void Initialize();
        void Update( CCommand& commandIn );
    private:
        // Timers
        orutil::CTimer m_statusTimer;
        int m_counter;
};
