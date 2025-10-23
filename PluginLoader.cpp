#include "PluginLoader.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>

PluginLoader::~PluginLoader() {
    for (auto& p : plugins) {
        if (p.handle) FreeLibrary(p.handle);
    }
}

void PluginLoader::loadPlugins(const std::string& directory) {
    std::filesystem::path pluginsDir(directory);

    if (!std::filesystem::exists(pluginsDir)) {
        std::cerr << "Папка плагинов не найдена: " << pluginsDir << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for (const auto& entry : std::filesystem::directory_iterator(pluginsDir)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".dll")
            continue;

        HMODULE hModule = LoadLibraryW(entry.path().wstring().c_str());
        if (!hModule) {
            std::cerr << "Ошибка загрузки DLL: " << entry.path().filename() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        auto func = reinterpret_cast<Plugin::FuncPtr>(GetProcAddress(hModule, "func"));
        auto getName = reinterpret_cast<const char* (*)()>(GetProcAddress(hModule, "getName"));

        if (!func || !getName) {
            std::cerr << "Ошибка: отсутствует func() или getName() в " << entry.path().filename() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        const char* name = nullptr;
        try {
            name = getName();
        }
        catch (...) {
            std::cerr << "Исключение при вызове getName() в " << entry.path().filename() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        if (!name || std::string(name).empty()) {
            std::cerr << "Плагин вернул пустое имя: " << entry.path().filename() << std::endl;
            std::exit(EXIT_FAILURE);
        }

        plugins.push_back({ name, hModule, func });
    }

    if (plugins.empty()) {
        std::cerr << "Не найдено ни одной корректной DLL." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "Библиотеки успешно подключены." << std::endl;
}

std::vector<std::string> PluginLoader::getOperatorNames() const {
    std::vector<std::string> names;
    for (const auto& p : plugins) {
        names.push_back(p.name);
    }
    return names;
}

const std::vector<PluginLoader::Plugin>& PluginLoader::getPlugins() const {
    return plugins;
}
