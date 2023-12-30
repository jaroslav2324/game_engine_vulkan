#include "../include/eng/logging.hpp"

void setConsoleTextColour(engConsoleTextColour colour){
    switch (colour)
    {
    case ENG_CONSOLE_TEXT_COLOUR_RED:     ENG_SET_CONSOLE_TEXT_COLOUR_RED     break;
    case ENG_CONSOLE_TEXT_COLOUR_YELLOW:  ENG_SET_CONSOLE_TEXT_COLOUR_YELLOW  break;
    case ENG_CONSOLE_TEXT_COLOUR_MAGENTA: ENG_SET_CONSOLE_TEXT_COLOUR_MAGENTA break;
    case ENG_CONSOLE_TEXT_COLOUR_CYAN:    ENG_SET_CONSOLE_TEXT_COLOUR_CYAN    break;
    default:                              ENG_SET_CONSOLE_TEXT_COLOUR_DEFAULT break;
    }
}

void printLogMessageType(engLogMessageType msgType){
    switch (msgType)
    {
    case ENG_LOG_MESSAGE_TYPE_VERBOSE:  printf("VERBOSE MESSAGE:");      break;
    case ENG_LOG_MESSAGE_TYPE_INFO:     printf("INFO:");                 break;
    case ENG_LOG_MESSAGE_TYPE_WARNING:  printf("WARNING:");              break;
    case ENG_LOG_MESSAGE_TYPE_ERROR:    printf("ERROR:");                break;
    default:                            printf("UNKNOWN MESSAGE TYPE:"); break;
    }
}

void logMessage(const char* message, engConsoleTextColour textColour, bool setDefaultColourAfterLogging, 
                engLogMessageType messageType, const char* inFile, int onLine){

    setConsoleTextColour(textColour);
    printLogMessageType(messageType);
    printf(" %s - file \"%s\", line %i\n", message, inFile, onLine);
    if (setDefaultColourAfterLogging){
        ENG_SET_CONSOLE_TEXT_COLOUR_DEFAULT
    }
}