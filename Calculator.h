#pragma once
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "PluginLoader.h"
#include <string>
#include <vector>
#include <unordered_map>

class Calculator {
public:
    Calculator() = default;
    ~Calculator() = default;

    void setPlugins(const std::vector<PluginLoader::Plugin>& plugins);

    double solve(const std::vector<std::string>& postfix);

private:
    struct PluginInfo {
        HMODULE handle;
        PluginLoader::Plugin::FuncPtr func;
    };

    std::unordered_map<std::string, PluginInfo> loadedFunctions;

    bool isOperator(const std::string& token) const;
};

#endif
