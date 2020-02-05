#pragma once
#include "SysConfig.h"


// ---------------------------------------
// Conditional module definitions
// -------------------------------
// Once these objects are instantiated, they will register themselves 
// as modules in the module manager.
// They can also be accessed individually as namespace members.
// ---------------------------------------
#if( HAS_SOME_MODULE )
#include "CSome_module.h"
CSome_module  m_some_module;
#endif
