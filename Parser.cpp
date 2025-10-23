#include "Parser.h"
#include <cctype>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <algorithm>

void Parser::setAllowedOperators(const std::vector<std::string>& ops) {
    allowedOperators = ops;
}

bool Parser::isNumber(const std::string& token) const {
    if (token.empty()) return false;
    char* end;
    std::strtod(token.c_str(), &end);
    return *end == '\0';
}

bool Parser::isOperator(const std::string& token) const {
    return std::find(allowedOperators.begin(), allowedOperators.end(), token) != allowedOperators.end();
}

int Parser::getPrecedence(const std::string& op) const {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    return 4;
}

bool Parser::isLeftAssociative(const std::string& op) const {
    return op != "^";
}

std::vector<std::string> Parser::tokenize(const std::string& expression) const {
    std::vector<std::string> tokens;
    std::string token;
    for (size_t i = 0; i < expression.size();) {
        if (std::isspace(expression[i])) {
            ++i;
            continue;
        }
        if (std::isdigit(expression[i]) || (expression[i] == '.' && i + 1 < expression.size() && std::isdigit(expression[i + 1]))) {
            token.clear();
            while (i < expression.size() && (std::isdigit(expression[i]) || expression[i] == '.')) {
                token.push_back(expression[i++]);
            }
            tokens.push_back(token);
            continue;
        }
        if (std::isalpha(expression[i])) {
            token.clear();
            while (i < expression.size() && std::isalpha(expression[i])) {
                token.push_back(expression[i++]);
            }
            tokens.push_back(token);
            continue;
        }
        if (std::string("()+-*/^").find(expression[i]) != std::string::npos) {
            tokens.emplace_back(1, expression[i++]);
            continue;
        }
        throw std::runtime_error("Недопустимый символ в выражении");
    }
    return tokens;
}

void Parser::validate(const std::string& expression) const {
    if (expression.empty())
        throw std::runtime_error("Пустое выражение");

    int parenBalance = 0;
    bool expectOperand = true;

    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];

        if (std::isspace(static_cast<unsigned char>(c)))
            continue;

        if (c == '(') {
            parenBalance++;
            expectOperand = true;
        }
        else if (c == ')') {
            parenBalance--;
            if (parenBalance < 0)
                throw std::runtime_error("Лишняя закрывающая скобка");
            expectOperand = false;
        }
        else if (std::isdigit(c) || c == '.') {
            while (i + 1 < expression.size() && (std::isdigit(expression[i + 1]) || expression[i + 1] == '.'))
                ++i;
            expectOperand = false;
        }
        else {
            bool matched = false;
            for (const auto& op : allowedOperators) {
                size_t len = op.size();
                if (expression.substr(i, len) == op) {
                    matched = true;
                    i += len - 1;
                    expectOperand = true;
                    break;
                }
            }
            if (!matched && (c == '-' || c == '+')) {
                if (expectOperand)
                    continue;
            }
            else if (!matched) {
                throw std::runtime_error(std::string("Неизвестный оператор или символ: ") + c);
            }
        }
    }

    if (parenBalance != 0)
        throw std::runtime_error("Несбалансированные скобки");
}

std::vector<std::string> Parser::toPostfix(const std::string& expression) const {
    std::vector<std::string> output;
    std::stack<std::string> ops;
    std::istringstream iss(expression);
    std::string token;

    auto precedence = [](const std::string& op) {
        if (op == "+" || op == "-") return 1;
        if (op == "*" || op == "/") return 2;
        if (op == "^") return 3;
        return 4;
        };

    auto isFunction = [&](const std::string& op) {
        return (op == "sin" || op == "cos" || op == "ln" || op == "sqrt");
        };

    for (size_t i = 0; i < expression.size();) {
        char c = expression[i];

        if (std::isspace(static_cast<unsigned char>(c))) {
            ++i;
            continue;
        }

        if (std::isdigit(c) || c == '.' ||
            (c == '-' && (i == 0 || expression[i - 1] == '(' || isFunction(std::string(1, expression[i - 1]))))) {
            size_t start = i;
            ++i;
            while (i < expression.size() && (std::isdigit(expression[i]) || expression[i] == '.'))
                ++i;
            output.push_back(expression.substr(start, i - start));
            continue;
        }

        if (std::isalpha(c)) {
            std::string func;
            while (i < expression.size() && std::isalpha(expression[i]))
                func.push_back(expression[i++]);
            ops.push(func);
            continue;
        }

        if (c == '(') {
            ops.push("(");
            ++i;
            continue;
        }

        if (c == ')') {
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top());
                ops.pop();
            }
            if (ops.empty())
                throw std::runtime_error("Лишняя закрывающая скобка");
            ops.pop();
            if (!ops.empty() && isFunction(ops.top())) {
                output.push_back(ops.top());
                ops.pop();
            }
            ++i;
            continue;
        }

        std::string op(1, c);
        while (!ops.empty() && precedence(ops.top()) >= precedence(op)) {
            output.push_back(ops.top());
            ops.pop();
        }
        ops.push(op);
        ++i;
    }

    while (!ops.empty()) {
        if (ops.top() == "(" || ops.top() == ")")
            throw std::runtime_error("Несбалансированные скобки");
        output.push_back(ops.top());
        ops.pop();
    }

    return output;
}
