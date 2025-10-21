#include <cmath>
#include <stdexcept>

extern "C" __declspec(dllexport) double func(double a, double b) {
    if (b == 0) throw std::runtime_error("Деление на ноль");
    return a / b;
}
