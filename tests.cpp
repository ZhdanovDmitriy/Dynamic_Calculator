#include "Calculator.h"
#include <cassert>
#include <iostream>

void runTests() {
#ifdef _DEBUG
    std::cout << "Юнит тестирование" << std::endl;

    Calculator calc;

    try {
        double res1 = calc.evaluate("1+1");
        assert(res1 == 0.0);
        std::cout << "Тест 1. (1+1) - успешно" << std::endl;
    }
    catch (...) {
        std::cerr << "Тест 1. (1+1) - ошибка" << std::endl;
        assert(false);
    }

    std::cout << "Все тесты пройдены." << std::endl;
    std::cout << "===========================" << std::endl;
#endif
}
