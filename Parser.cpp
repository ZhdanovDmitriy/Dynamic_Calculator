#include "Parser.h"
#include <cctype>
#include <stack>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

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
    size_t i = 0;

    while (i < expression.size()) {
        if (std::isspace(expression[i])) { i++; continue; }

        if (std::isdigit(expression[i]) || (expression[i] == '.' && i + 1 < expression.size() && std::isdigit(expression[i + 1]))) {
            size_t start = i;
            while (i < expression.size() && (std::isdigit(expression[i]) || expression[i] == '.')) i++;
            tokens.push_back(expression.substr(start, i - start));
            continue;
        }

        if (std::isalpha(expression[i])) {
            size_t start = i;
            while (i < expression.size() && std::isalpha(expression[i])) i++;
            tokens.push_back(expression.substr(start, i - start));
            continue;
        }

        if (std::string("()+-*/^").find(expression[i]) != std::string::npos) {
            if (expression[i] == '-') {
                if (tokens.empty() || tokens.back() == "(" || isOperator(tokens.back())) {
                    tokens.push_back("u-");
                    i++;
                    continue;
                }
            }
            tokens.push_back(std::string(1, expression[i]));
            i++;
            continue;
        }

        throw std::runtime_error("Недопустимый символ в выражении");
    }

    return tokens;
}

void Parser::validate(const std::string& expression) const {
    auto tokens = tokenize(expression);
    int parenBalance = 0;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string& t = tokens[i];

        if (t == "(") parenBalance++;
        else if (t == ")") {
            parenBalance--;
            if (parenBalance < 0) throw std::runtime_error("Лишняя закрывающая скобка");
        }
        else if (isOperator(t) || t == "u-") {
            if (t != "u-" && !isOperator(t)) throw std::runtime_error("Неизвестный оператор: " + t);
        }
        else {
            if (!isNumber(t) && !isOperator(t)) {
                if (std::find(allowedOperators.begin(), allowedOperators.end(), t) == allowedOperators.end())
                    throw std::runtime_error("Неизвестная функция: " + t);
            }
        }
    }

    if (parenBalance != 0) throw std::runtime_error("Несбалансированные скобки");
}

std::vector<std::string> Parser::toPostfix(const std::string& expression) const {
    std::vector<std::string> output;
    std::stack<std::string> ops;
    auto tokens = tokenize(expression);

    auto precedence = [this](const std::string& op) { return getPrecedence(op); };
    auto isFunction = [this](const std::string& op) { return isOperator(op) && getPrecedence(op) == 4; };

    for (const auto& token : tokens) {
        if (isNumber(token)) {
            output.push_back(token);
        }
        else if (token == "u-") {
            ops.push(token);
        }
        else if (isFunction(token)) {
            ops.push(token);
        }
        else if (isOperator(token)) {
            while (!ops.empty() && ((isFunction(ops.top())) ||
                (precedence(ops.top()) > precedence(token)) ||
                (precedence(ops.top()) == precedence(token) && isLeftAssociative(token)))) {
                output.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        }
        else if (token == "(") {
            ops.push(token);
        }
        else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top());
                ops.pop();
            }
            if (ops.empty()) throw std::runtime_error("Лишняя закрывающая скобка");
            ops.pop();
            if (!ops.empty() && (isFunction(ops.top()) || ops.top() == "u-")) {
                output.push_back(ops.top());
                ops.pop();
            }
        }
        else {
            throw std::runtime_error("Неизвестный токен: " + token);
        }
    }

    while (!ops.empty()) {
        if (ops.top() == "(" || ops.top() == ")") throw std::runtime_error("Несбалансированные скобки");
        output.push_back(ops.top());
        ops.pop();
    }

    return output;
}
