#include "Calculator.h"
#include <stdexcept>
#include <stack>
#include <filesystem>
#include <iostream>

Calculator::Calculator() {
    loadPlugins();
}

Calculator::~Calculator() {
    for (auto& [name, plugin] : loadedFunctions) {
        if (plugin.handle) FreeLibrary(plugin.handle);
    }
}

void Calculator::loadPlugins() {
#ifdef _DEBUG
    std::filesystem::path pluginsDir = "plugins/debug";
#else
    std::filesystem::path pluginsDir = "plugins/release";
#endif

    for (const auto& entry : std::filesystem::directory_iterator(pluginsDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            HMODULE hModule = LoadLibraryW(entry.path().wstring().c_str());
            if (!hModule) {
                std::cerr << "Ошибка загрузки " << entry.path().filename() << std::endl;
                std::exit(EXIT_FAILURE);
            }

            auto func = reinterpret_cast<FuncPtr>(GetProcAddress(hModule, "func"));
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

            if (!name || !*name) {
                std::cerr << "Ошибка: плагин " << entry.path().filename() << " вернул пустое имя" << std::endl;
                std::exit(EXIT_FAILURE);
            }

            Plugin p{ hModule, func };
            loadedFunctions[name] = p;
        }
    }

    if (loadedFunctions.empty()) {
        std::cerr << "Не найдено ни одной DLL с функцией" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

bool Calculator::isOperator(const std::string& token) const {
    return loadedFunctions.find(token) != loadedFunctions.end();
}

double Calculator::solve(const std::vector<std::string>& postfix) {
    if (postfix.empty())
        throw std::runtime_error("Пустое выражение");

    std::stack<double> stack;

    for (const auto& token : postfix) {
        if (!isOperator(token)) {
            char* end;
            double val = std::strtod(token.c_str(), &end);
            if (*end != '\0') throw std::runtime_error("Некорректное число: " + token);
            stack.push(val);
            continue;
        }

        auto it = loadedFunctions.find(token);
        if (it == loadedFunctions.end())
            throw std::runtime_error("Неизвестный оператор: " + token);

        auto func = it->second.func;

        double b = 0.0, a = 0.0;

        if (stack.empty())
            throw std::runtime_error("Недостаточно аргументов для оператора: " + token);

        b = stack.top();
        stack.pop();

        if (stack.empty()) {
            try {
                double res = func(b, 0.0);
                stack.push(res);
            }
            catch (const std::exception& ex) {
                throw std::runtime_error(std::string("Ошибка в функции '") + token + "': " + ex.what());
            }
            continue;
        }

        a = stack.top();
        stack.pop();

        try {
            double res = func(a, b);
            stack.push(res);
        }
        catch (const std::exception& ex) {
            throw std::runtime_error(std::string("Ошибка в функции '") + token + "': " + ex.what());
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Ошибка вычисления выражения");

    return stack.top();
}
