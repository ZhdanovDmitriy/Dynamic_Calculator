#pragma once
#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H

#include "Calculator.h"
#include "Parser.h"
#include "PluginLoader.h"
#include <string>

class ExpressionEvaluator {
public:
    ExpressionEvaluator(const std::string& pluginsDir = "");
    ~ExpressionEvaluator() = default;

    void run();

private:
    Parser parser;
    Calculator calc;
    PluginLoader loader;

    std::string determinePluginsDirectory(const std::string& pluginsDir) const;
    void initialize();
};

#endif
