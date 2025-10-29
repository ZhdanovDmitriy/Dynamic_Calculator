#include "Calculator.h"
#include <stack>
#include <stdexcept>
#include <cstdlib>
#include <cmath>
#include <sstream>

bool Calculator::isOperator(const std::string& token) const {
    return loadedFunctions.find(token) != loadedFunctions.end();
}

void Calculator::setPlugins(const std::vector<PluginLoader::Plugin>& plugins) {
    loadedFunctions.clear();
    for (const auto& p : plugins) {
        loadedFunctions[p.name] = { p.handle, p.func };
    }
}

double Calculator::solve(const std::vector<std::string>& postfix) {
    if (postfix.empty())
        throw std::runtime_error("Пустое выражение");

    std::stack<double> stack;

    for (const auto& token : postfix) {
        if (token == "u-") {
            if (stack.empty())
                throw std::runtime_error("Недостаточно аргументов для унарного минуса");
            double val = stack.top(); stack.pop();
            stack.push(-val);
            continue;
        }

        if (!isOperator(token)) {
            std::istringstream iss(token);
            iss.imbue(std::locale::classic());
            double val;
            if (!(iss >> val)) {
                throw std::runtime_error("Некорректное число: " + token);
            }
            char remaining;
            if (iss >> remaining) {
                throw std::runtime_error("Некорректное число: " + token);
            }
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
                if (std::isnan(res)) throw std::runtime_error("Функция '" + token + "' вернула NaN");
                if (std::isinf(res)) throw std::runtime_error("Функция '" + token + "' вернула бесконечность");
                stack.push(res);
            }
            catch (const std::exception& ex) {
                throw std::runtime_error("Ошибка в функции '" + token + "': " + ex.what());
            }
            continue;
        }

        a = stack.top();
        stack.pop();

        try {
            double res = func(a, b);
            if (std::isnan(res)) throw std::runtime_error("Функция '" + token + "' вернула NaN");
            if (std::isinf(res)) throw std::runtime_error("Функция '" + token + "' вернула бесконечность");
            stack.push(res);
        }
        catch (const std::exception& ex) {
            throw std::runtime_error("Ошибка в функции '" + token + "': " + ex.what());
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Ошибка вычисления выражения");

    return stack.top();
}
