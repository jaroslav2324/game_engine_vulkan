#pragma once

#ifdef _WIN32
    #define EXPORTLIB __declspec(dllexport)
#elif __linux__
    #define EXPORTLIB __attribute__((visibility("default")))
#endif

EXPORTLIB int test_sum(int a, int b);
