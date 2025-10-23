#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <filesystem>

typedef double(*FuncPtr)(double, double);

void runTests() {
#ifdef _DEBUG
    std::cout << "Текущий каталог: " << std::filesystem::current_path() << std::endl;
#endif

#ifdef _DEBUG
    std::filesystem::path pluginsDir = "plugins/debug";
#else
    std::filesystem::path pluginsDir = "plugins/release";
#endif

#ifdef _DEBUG
    std::cout << "Юнит тестирование:" << std::endl;
#endif

    bool allPluginsOk = true;

    for (const auto& entry : std::filesystem::directory_iterator(pluginsDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            const auto& pluginPath = entry.path();
#ifdef _DEBUG
            std::cout << "Проверяется DLL: " << pluginPath << std::endl;
#endif

            HMODULE hModule = LoadLibraryW(pluginPath.wstring().c_str());
            if (!hModule) {
#ifdef _DEBUG
                std::cerr << "Ошибка: не удалось загрузить " << pluginPath << std::endl;
#else
                std::cerr << "Ошибка загрузки DLL" << std::endl;
#endif
                allPluginsOk = false;
                continue;
            }

            FuncPtr func = reinterpret_cast<FuncPtr>(GetProcAddress(hModule, "func"));
            if (!func) {
#ifdef _DEBUG
                std::cerr << "Ошибка: функция 'func' не найдена в " << pluginPath << std::endl;
#else
                std::cerr << "Ошибка подключения функции DLL" << std::endl;
#endif
                FreeLibrary(hModule);
                allPluginsOk = false;
                continue;
            }

            FreeLibrary(hModule);
        }
    }

    if (allPluginsOk) {
        std::cout << "Все динамические библиотеки валидны." << std::endl;
    }
    else {
        std::cerr << "Не все динамические библиотеки валидны! Приложение будет завершено." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
