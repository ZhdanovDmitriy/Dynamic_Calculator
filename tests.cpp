#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

typedef double(*FuncPtr)(double, double);

struct TestCase {
    double a;
    double b;
    double expectedResult;
    bool expectException;
};

struct Test {
    std::string name;
    std::vector<TestCase> cases;
};

double callFunc(FuncPtr func, double a, double b) {
    return func(a, b);
}

void runTests() {
    std::vector<std::string> pluginFiles = {
        "plugins/funcAdd.dll",
        "plugins/funcSub.dll",
        "plugins/funcMul.dll",
        "plugins/funcDiv.dll",
        "plugins/funcPow.dll",
        "plugins/funcSqrt.dll",
        "plugins/funcSin.dll",
        "plugins/funcCos.dll",
        "plugins/funcLn.dll",
        "plugins/funcTg.dll",
        "plugins/funcCtg.dll",
        "plugins/funcMod.dll",
        "plugins/funcDivInt.dll"
    };

#ifdef _DEBUG
    std::cout << "Юнит тестирование:" << std::endl;
#endif

    for (size_t i = 0; i < pluginFiles.size(); ++i) {
        HMODULE hModule = LoadLibraryA(pluginFiles[i].c_str());
        if (!hModule) {
#ifdef _DEBUG
            std::cerr << "Ошибка: не удалось загрузить " << pluginFiles[i] << std::endl;
#else
            std::cerr << "Ошибка загрузки DLL" << std::endl;
#endif
            std::exit(EXIT_FAILURE);
        }

        FuncPtr func = (FuncPtr)GetProcAddress(hModule, "func");
        if (!func) {
#ifdef _DEBUG
            std::cerr << "Ошибка: функция 'func' не найдена в " << pluginFiles[i] << std::endl;
#else
            std::cerr << "Ошибка подключения функции DLL" << std::endl;
#endif
            FreeLibrary(hModule);
            continue;
        }
        FreeLibrary(hModule);
    }

    std::cout << "Все динамические библиотеки успешно подключены." << std::endl;
}
