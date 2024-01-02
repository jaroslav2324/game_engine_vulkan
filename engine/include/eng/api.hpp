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

// Vulkan validation layers
#ifdef NDEBUG
    const bool ENG_VALIDATION_LAYERS_ENABLED = false;
#else
    const bool ENG_VALIDATION_LAYERS_ENABLED = true;
#endif


typedef char      i8 ;
typedef short int i16;
typedef int       i32;
typedef long long i64;

typedef unsigned char      u8 ;
typedef unsigned short int u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef float  f32;
typedef double f64;


#include <cassert>

static_assert(sizeof(i8 ) == 1);
static_assert(sizeof(i16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(i64) == 8);

static_assert(sizeof(u8 ) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);

static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);



enum engResult{
    ENG_RESULT_SUCCESS,
    ENG_RESULT_FAILURE
};