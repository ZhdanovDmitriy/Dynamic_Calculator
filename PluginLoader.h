#pragma once
#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include <string>
#include <vector>
#include <windows.h>

class PluginLoader {
public:
    struct Plugin {
        std::string name;
        HMODULE handle;
        typedef double(*FuncPtr)(double, double);
        FuncPtr func;
    };

    PluginLoader() = default;
    ~PluginLoader();

    void loadPlugins(const std::string& directory);

    std::vector<std::string> getOperatorNames() const;

    const std::vector<Plugin>& getPlugins() const;

private:
    std::vector<Plugin> plugins;
};

#endif
