#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>

class Calculator {
public:
    Calculator() = default;
    ~Calculator() = default;
    double evaluate(const std::string& expr);
};

#endif
