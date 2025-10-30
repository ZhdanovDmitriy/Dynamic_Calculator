#include <cmath>
#include <stdexcept>

extern "C" __declspec(dllexport) double func(double a, double b) {
    (void)b;
    if (a <= 0) throw std::runtime_error("Логарифм от неположительного числа");
    return std::log(a);
}
