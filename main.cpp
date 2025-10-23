#include <iostream>
#include "ExpressionEvaluator.h"
#include "tests.h"
#include <locale>

int main() {
    setlocale(LC_ALL, "Russian");
    runTests();

    ExpressionEvaluator evaluator;

    std::cout << "Введите выражение или 'exit':" << std::endl;

    evaluator.run();

    std::cout << "Приложение корректно завершило свою работу" << std::endl;
    return 0;
}