#include <cmath>
#include <stdexcept>

extern "C" __declspec(dllexport) double func(double a, double b) {
    (void)b;
    if (std::sin(a) == 0) throw std::runtime_error("Котангенс не определён");
    return 1.0 / std::tan(a);
}
