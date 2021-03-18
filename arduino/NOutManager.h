#pragma once

// Includes
#include <Arduino.h>
#include <orutil.h>

// Commands
#define COMMAND_MAX_ARGUMENTS			10
#define COMMAND_MAX_STRING_LENGTH		40
#define COMMAND_MAX_COUNT				5	// Use 3 for 328p
#define COMMAND_DATA_BUFFER_SIZE		80

class OutManager
{
    public:
    OutManager( const char* moduleName );
    OutManager( const char* moduleName, size_t arraySize);
    void Append( const char* argument );
    void Reset();
    bool Send();
    bool operator=( const char* outputStringComp );
    char* operator[]( size_t index );
    private:
    const char moduleName[COMMAND_DATA_BUFFER_SIZE];
    size_t currentIndex{};
    char m_arguments[ COMMAND_MAX_ARGUMENTS ][ COMMAND_DATA_BUFFER_SIZE ]; // 2d array (array of strings)
    void Parse();

}