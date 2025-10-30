#include <cmath>
#include <stdexcept>

extern "C" __declspec(dllexport) double func(double a, double b) {
    (void)b;
    if (a < 0) throw std::runtime_error("Ќельз€ вз€ть корень из отрицательного числа");
    return std::sqrt(a);
}
