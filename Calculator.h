#pragma once
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>
#include <unordered_map>
#include <windows.h>

class Calculator {
public:
    Calculator();
    ~Calculator();

    double solve(const std::vector<std::string>& postfix);

private:
    typedef double (*FuncPtr)(double, double);

    struct Plugin {
        HMODULE handle;
        FuncPtr func;
    };

    std::unordered_map<std::string, Plugin> loadedFunctions;

    void loadPlugins();
    bool isOperator(const std::string& token) const;
};

#endif
