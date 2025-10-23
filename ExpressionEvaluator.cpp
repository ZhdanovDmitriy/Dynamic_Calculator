#include "ExpressionEvaluator.h"
#include <iostream>

ExpressionEvaluator::ExpressionEvaluator() {
}

void ExpressionEvaluator::run() {
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
            std::cerr << "Îøèáêà: " << ex.what() << std::endl;
        }
    }
}
