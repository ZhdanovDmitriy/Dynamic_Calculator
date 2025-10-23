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
    auto tokens = tokenize(expression);
    if (tokens.empty()) throw std::runtime_error("Пустое выражение");

    int bracketCount = 0;
    bool expectNumber = true;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& tok = tokens[i];
        if (tok == "(") {
            bracketCount++;
            expectNumber = true;
        }
        else if (tok == ")") {
            bracketCount--;
            if (bracketCount < 0) throw std::runtime_error("Лишняя закрывающая скобка");
            expectNumber = false;
        }
        else if (isNumber(tok)) {
            if (!expectNumber) throw std::runtime_error("Неверная структура выражения");
            expectNumber = false;
        }
        else if (isOperator(tok)) {
            if (expectNumber) throw std::runtime_error("Неверная структура выражения");
            expectNumber = true;
        }
        else {
            throw std::runtime_error("Неизвестный оператор или символ: " + tok);
        }
    }

    if (bracketCount != 0)
        throw std::runtime_error("Несбалансированные скобки");

    if (expectNumber)
        throw std::runtime_error("Выражение не может заканчиваться оператором");
}

std::vector<std::string> Parser::toPostfix(const std::string& expression) const {
    auto tokens = tokenize(expression);
    std::vector<std::string> output;
    std::stack<std::string> stack;

    for (const auto& token : tokens) {
        if (isNumber(token)) {
            output.push_back(token);
        }
        else if (isOperator(token)) {
            while (!stack.empty() && isOperator(stack.top()) &&
                ((isLeftAssociative(token) && getPrecedence(token) <= getPrecedence(stack.top())) ||
                    (!isLeftAssociative(token) && getPrecedence(token) < getPrecedence(stack.top())))) {
                output.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        }
        else if (token == "(") {
            stack.push(token);
        }
        else if (token == ")") {
            while (!stack.empty() && stack.top() != "(") {
                output.push_back(stack.top());
                stack.pop();
            }
            if (stack.empty()) throw std::runtime_error("Несбалансированные скобки");
            stack.pop();
        }
        else {
            throw std::runtime_error("Неизвестный токен: " + token);
        }
    }

    while (!stack.empty()) {
        if (stack.top() == "(" || stack.top() == ")")
            throw std::runtime_error("Несбалансированные скобки");
        output.push_back(stack.top());
        stack.pop();
    }

    return output;
}
