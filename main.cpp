#include <iostream>
#include <string>
#include "Calculator.h"
#include "tests.h" 
#include <locale>

int main() {
    setlocale(LC_ALL, "Russian");
    runTests();

    std::cout << "Введите выражение или 'exit':" << std::endl;

    Calculator calc;
    std::string input;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) {
            break;
        }
        if (input == "exit") {
            break;
        }

        try {
            double result = calc.solve(input);
            std::cout << "= " << result << std::endl;
        }
        catch (const std::exception& ex) {
            std::cerr << "Ошибка: " << ex.what() << std::endl;
        }
    }

    std::cout << "Приложение корректно завершило свою работу" << std::endl;
    return 0;
}
