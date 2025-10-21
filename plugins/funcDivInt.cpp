#include <cmath>
#include <stdexcept>

extern "C" __declspec(dllexport) double func(double a, double b) {
    if (b == 0) throw std::runtime_error("Деление на ноль в div");
    return static_cast<int>(a) / static_cast<int>(b);
}
