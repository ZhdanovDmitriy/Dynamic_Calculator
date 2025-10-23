#include "ExpressionEvaluator.h"
#include <iostream>
#include <filesystem>

ExpressionEvaluator::ExpressionEvaluator(const std::string& pluginsDir) {
    std::string dir = determinePluginsDirectory(pluginsDir);

    loader.loadPlugins(dir);

    parser.setAllowedOperators(loader.getOperatorNames());
    calc.setPlugins(loader.getPlugins());
}

std::string ExpressionEvaluator::determinePluginsDirectory(const std::string& pluginsDir) const {
    if (!pluginsDir.empty()) return pluginsDir;

#ifdef _DEBUG
    return "plugins/debug";
#else
    return "plugins/release";
#endif
}

void ExpressionEvaluator::run() {
    std::cout << "¬ведите выражение или 'exit':" << std::endl;
    std::string input;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) break;
        if (input == "exit") break;

        try {
            parser.validate(input);
            auto postfix = parser.toPostfix(input);
            double result = calc.solve(postfix);
            std::cout << "= " << result << std::endl;
        }
        catch (const std::exception& ex) {
            std::cerr << "ќшибка: " << ex.what() << std::endl;
        }
    }
}
