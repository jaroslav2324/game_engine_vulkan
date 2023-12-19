#include "../include/eng/api.hpp"

int CALLCDECL test_sum(int a, int b){
    return a + b;
}

int CALLCDECL test_min(int a, int b){
    return (a < b) ? a : b;
}