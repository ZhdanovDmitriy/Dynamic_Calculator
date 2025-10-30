#include <cmath>

extern "C" __declspec(dllexport) double func(double a, double b) {
    return a - b;
}
