#pragma once
#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H

#include "Calculator.h"
#include "Parser.h"
#include <string>

class ExpressionEvaluator {
public:
    ExpressionEvaluator();
    ~ExpressionEvaluator() = default;

    void run();

private:
    Calculator calc;
    Parser parser;
};

#endif
