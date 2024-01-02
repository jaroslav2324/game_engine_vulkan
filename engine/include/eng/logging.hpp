#pragma once
#include <stdio.h>

#ifdef __linux__

#define ENG_SET_CONSOLE_TEXT_COLOUR_DEFAULT     printf("\033[39m");
#define ENG_SET_CONSOLE_TEXT_COLOUR_RED         printf("\033[91m");
#define ENG_SET_CONSOLE_TEXT_COLOUR_YELLOW      printf("\033[93m");
#define ENG_SET_CONSOLE_TEXT_COLOUR_MAGENTA     printf("\033[95m");
#define ENG_SET_CONSOLE_TEXT_COLOUR_CYAN        printf("\033[96m");
#define ENG_SET_CONSOLE_TEXT_COLOUR_DARK_GREY   printf("\033[90m");

// ! Win32 not supported now
#elif _WIN32

#endif


enum engConsoleTextColour{
    ENG_CONSOLE_TEXT_COLOUR_DEFAULT,
    ENG_CONSOLE_TEXT_COLOUR_RED,
    ENG_CONSOLE_TEXT_COLOUR_YELLOW,
    ENG_CONSOLE_TEXT_COLOUR_MAGENTA,
    ENG_CONSOLE_TEXT_COLOUR_CYAN,
    ENG_CONSOLE_TEXT_COLOUR_DARK_GRAY
};

enum engLogMessageType{
    ENG_LOG_MESSAGE_TYPE_VERBOSE,
    ENG_LOG_MESSAGE_TYPE_INFO,
    ENG_LOG_MESSAGE_TYPE_WARNING,
    ENG_LOG_MESSAGE_TYPE_ERROR
};

void setConsoleTextColour(engConsoleTextColour colour);
void printLogMessageType(engLogMessageType msgType);

// pass to logMessage function instead of inFile arg to skip filename output
#define ENG_LOG_NO_FILENAME nullptr
// pass to logMessage function instead of onLine arg to skip line number output
#define ENG_LOG_NO_LINE_NUMBER 0

// Use macros like ENG_LOG_VERBOSE, they work only in debug configuration.
// This function would work both in debug and release config
void logMessage(const char* message, engConsoleTextColour textColour, bool setDefaultColourAfterLogging, 
                engLogMessageType messageType, const char* inFile, int onLine);


#ifdef NDEBUG

    #define ENG_LOG_VERBOSE(message)
    #define ENG_LOG_INFO(message)
    #define ENG_LOG_INFO_MAGENTA(message)
    #define ENG_LOG_INFO_CYAN(message)
    #define ENG_LOG_WARNING(message)
    #define ENG_LOG_ERROR(message)

#else

    #define ENG_LOG_VERBOSE(message) {const char* pMessage = message;\
                                    logMessage(pMessage, ENG_CONSOLE_TEXT_COLOUR_DEFAULT, false, \
                                                ENG_LOG_MESSAGE_TYPE_VERBOSE, ENG_LOG_NO_FILENAME, ENG_LOG_NO_LINE_NUMBER);}

    #define ENG_LOG_INFO(message)    {const char* pMessage = message;\
                                    const char* pFilename = __FILE__;\
                                    logMessage(pMessage, ENG_CONSOLE_TEXT_COLOUR_DEFAULT, false, \
                                                ENG_LOG_MESSAGE_TYPE_INFO, ENG_LOG_NO_FILENAME, ENG_LOG_NO_LINE_NUMBER);}

    #define ENG_LOG_INFO_MAGENTA(message)    {const char* pMessage = message;\
                                    const char* pFilename = __FILE__;\
                                    logMessage(pMessage, ENG_CONSOLE_TEXT_COLOUR_MAGENTA, true, \
                                                ENG_LOG_MESSAGE_TYPE_INFO, ENG_LOG_NO_FILENAME, ENG_LOG_NO_LINE_NUMBER);}

    #define ENG_LOG_INFO_CYAN(message)    {const char* pMessage = message;\
                                    const char* pFilename = __FILE__;\
                                    logMessage(pMessage, ENG_CONSOLE_TEXT_COLOUR_CYAN, true, \
                                                ENG_LOG_MESSAGE_TYPE_INFO, ENG_LOG_NO_FILENAME, ENG_LOG_NO_LINE_NUMBER);}

    #define ENG_LOG_WARNING(message) {const char* pMessage = message;\
                                    const char* pFilename = __FILE__;\
                                    logMessage(pMessage, ENG_CONSOLE_TEXT_COLOUR_YELLOW, true, \
                                                ENG_LOG_MESSAGE_TYPE_WARNING, pFilename, __LINE__);}

    #define ENG_LOG_ERROR(message)   {const char* pMessage = message;\
                                    const char* pFilename = __FILE__;\
                                    logMessage(pMessage, ENG_CONSOLE_TEXT_COLOUR_RED, true, \
                                                ENG_LOG_MESSAGE_TYPE_ERROR, pFilename, __LINE__);}                                  

#endif