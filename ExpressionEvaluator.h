#pragma once
#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H

#include "Calculator.h"
#include "Parser.h"
#include <string>
#include <vector>

class ExpressionEvaluator {
public:
    ExpressionEvaluator();
    ~ExpressionEvaluator() = default;

    void run();

private:
    std::vector<std::string> loadPlugins();

    Calculator calc;
    Parser parser;
};

#endif
