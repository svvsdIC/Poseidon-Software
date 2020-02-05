#pragma once

// Includes
#include <Arduino.h>
#include <orutil.h>

// Data structures

// Shared data namespace
namespace NDataManager
{
    extern orutil::CTimer		m_timer_1hz;
    extern orutil::CTimer	    m_timer_10hz;

    extern uint32_t				m_loopsPerSec;

    extern void Initialize();
    extern void HandleOutputLoops();
}
