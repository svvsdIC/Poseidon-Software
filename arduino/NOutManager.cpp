#pragma once

// Includes
#include <Arduino.h>
#include <string.h>
#include <orutil.h>
#include "NOutManager.h"

#if(HAS_OUT_MAN)


OutManager::OutManager( const char* moduleName ) // if we had access to strings this would be so much easier!
{
    memset(moduleName, '\0', COMMAND_DATA_BUFFER_SIZE);
    strncpy(this.moduleName, )
    
}

OutManager::OutManager( const char* moduleName, size_t arraySize)
{

}

void OutManager::Append( const char* arg )
{

}

void OutManager::Reset()
{

}

void OutManager::Send()
{

}

bool OutManager::operator=( const char* outputStringComp )
{

}

char* OutManager::operator[]( size_t index )
{

}

void OutManager::Parse() // do we need this? lets find out!
{

}

#endif /* HAS_OUT_MAN */