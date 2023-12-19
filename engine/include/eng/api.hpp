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

#ifdef NDEBUG
    const bool ENG_VALIDATION_LAYERS_ENABLED = false;
#else
    const bool ENG_VALIDATION_LAYERS_ENABLED = true;
#endif
