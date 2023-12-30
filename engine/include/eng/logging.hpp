#pragma once
#include <stdio.h>

#ifdef __linux__

#define ENG_SET_CONSOLE_TEXT_COLOUR_DEFAULT printf("\033[39m");
#define ENG_SET_CONSOLE_TEXT_COLOUR_RED     printf("\033[91m");
#define ENG_SET_CONSOLE_TEXT_COLOUR_YELLOW  printf("\033[93m");
#define ENG_SET_CONSOLE_TEXT_COLOUR_MAGENTA printf("\033[95m");
#define ENG_SET_CONSOLE_TEXT_COLOUR_CYAN    printf("\033[96m");

#elif _WIN32

#endif


enum engConsoleTextColour{
    ENG_CONSOLE_TEXT_COLOUR_DEFAULT,
    ENG_CONSOLE_TEXT_COLOUR_RED,
    ENG_CONSOLE_TEXT_COLOUR_YELLOW,
    ENG_CONSOLE_TEXT_COLOUR_MAGENTA,
    ENG_CONSOLE_TEXT_COLOUR_CYAN
};

enum engLogMessageType{
    ENG_LOG_MESSAGE_TYPE_VERBOSE,
    ENG_LOG_MESSAGE_TYPE_INFO,
    ENG_LOG_MESSAGE_TYPE_WARNING,
    ENG_LOG_MESSAGE_TYPE_ERROR
};

void setConsoleTextColour(engConsoleTextColour colour);
void printLogMessageType(engLogMessageType msgType);

void logMessage(const char* message, engConsoleTextColour textColour, bool setDefaultColourAfterLogging, 
                engLogMessageType messageType, const char* inFile, int onLine);

#define ENG_LOG_VERBOSE(message) const char* pMessage = message;\
                                 const char* pFilename = __FILE__;\
                                 logMessage(pMessage, ENG_CONSOLE_TEXT_COLOUR_DEFAULT, false, \
                                             ENG_LOG_MESSAGE_TYPE_VERBOSE, pFilename, __LINE__);

#define ENG_LOG_INFO(message)    const char* pMessage = message;\
                                 const char* pFilename = __FILE__;\
                                 logMessage(pMessage, ENG_CONSOLE_TEXT_COLOUR_DEFAULT, false, \
                                             ENG_LOG_MESSAGE_TYPE_INFO, pFilename, __LINE__);

#define ENG_LOG_WARNING(message) const char* pMessage = message;\
                                 const char* pFilename = __FILE__;\
                                 logMessage(pMessage, ENG_CONSOLE_TEXT_COLOUR_YELLOW, true, \
                                             ENG_LOG_MESSAGE_TYPE_WARNING, pFilename, __LINE__);

#define ENG_LOG_ERROR(message)   const char* pMessage = message;\
                                 const char* pFilename = __FILE__;\
                                 logMessage(pMessage, ENG_CONSOLE_TEXT_COLOUR_RED, true, \
                                             ENG_LOG_MESSAGE_TYPE_ERROR, pFilename, __LINE__);                                  
