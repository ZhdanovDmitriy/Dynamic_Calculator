#include <iostream>
#include "ExpressionEvaluator.h"
#include "tests.h"
#include <locale>
#include <filesystem>

int main() {
    setlocale(LC_ALL, "Russian");
    runTests();

    ExpressionEvaluator evaluator;
    evaluator.run();

    return 0;
}