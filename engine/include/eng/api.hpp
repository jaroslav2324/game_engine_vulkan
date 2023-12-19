#pragma once

// ! Windows not supported now
#ifdef _WIN32
    #ifdef ENGINE_EXPORTS
        #define EXPORTLIB __declspec(dllexport)
    #else
        #define EXPORTLIB __declspec(dllimport)
    #endif
#elif __linux__
    #ifdef ENGINE_EXPORTS
        #define EXPORTLIB __attribute__((visibility("default")))
    #else
        #define EXPORTLIB 
    #endif
#endif


#define CALLCDECL __cdecl

EXPORTLIB int CALLCDECL test_sum(int a, int b);
EXPORTLIB int CALLCDECL test_min(int a, int b);

