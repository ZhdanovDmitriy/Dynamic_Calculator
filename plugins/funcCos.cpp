#include <cmath>

extern "C" __declspec(dllexport) double func(double a, double b) {
    (void)b;
    return std::cos(a);
}
