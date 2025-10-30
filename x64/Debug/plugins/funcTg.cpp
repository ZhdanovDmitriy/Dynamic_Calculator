#include <cmath>
#include <stdexcept>

extern "C" __declspec(dllexport) double func(double a, double b) {
    (void)b;
    if (std::cos(a) == 0) throw std::runtime_error("Тангенс не определён");
    return std::tan(a);
}
