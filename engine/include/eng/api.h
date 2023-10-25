#pragma once

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


#ifdef __cplusplus
extern "C"
{
#endif

EXPORTLIB int CALLCDECL test_sum(int a, int b);
EXPORTLIB int CALLCDECL test_min(int a, int b);

#ifdef __cplusplus
}
#endif