#include "ExpressionEvaluator.h"
#include <iostream>
#include <filesystem>
#include <windows.h>
#include <vector>
#include <string>

typedef const char* (*GetNamePtr)();

std::vector<std::string> ExpressionEvaluator::loadPlugins() {
    std::vector<std::string> availableOperators;

#ifdef _DEBUG
    std::filesystem::path pluginsDir = "plugins/debug";
#else
    std::filesystem::path pluginsDir = "plugins/release";
#endif

    if (!std::filesystem::exists(pluginsDir)) {
        std::cerr << "Папка плагинов не найдена: " << pluginsDir << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for (const auto& entry : std::filesystem::directory_iterator(pluginsDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            HMODULE hModule = LoadLibraryW(entry.path().wstring().c_str());
            if (!hModule) {
                std::cerr << "Ошибка загрузки DLL: " << entry.path().filename() << std::endl;
                std::exit(EXIT_FAILURE);
            }

            GetNamePtr getName = reinterpret_cast<GetNamePtr>(GetProcAddress(hModule, "getName"));
            if (!getName) {
                std::cerr << "Ошибка: функция getName() не найдена в " << entry.path().filename() << std::endl;
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

            availableOperators.emplace_back(name);
            FreeLibrary(hModule);
        }
    }

    if (availableOperators.empty()) {
        std::cerr << "Не найдено ни одной корректной DLL." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return availableOperators;
}

void ExpressionEvaluator::run() {
    auto operators = loadPlugins();

    std::string input;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) break;
        if (input == "exit") break;

        try {
            auto tokens = parser.tokenize(input);
            parser.validate(tokens);
            double result = calc.solve(input);
            std::cout << "= " << result << std::endl;
        }
        catch (const std::exception& ex) {
            std::cerr << "Ошибка: " << ex.what() << std::endl;
        }
    }
}
