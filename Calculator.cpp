#include "Calculator.h"
#include <stdexcept>
#include <string>

double Calculator::evaluate(const std::string& expr) {
    if (expr.empty()) {
        throw std::runtime_error("Пустое выражение");
    }
    return 0.0;
}
