#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class Parser {
public:
    Parser() = default;
    ~Parser() = default;

    void setAllowedOperators(const std::vector<std::string>& ops);

    void validate(const std::string& expression) const;
    std::vector<std::string> toPostfix(const std::string& expression) const;

private:
    std::vector<std::string> allowedOperators;

    bool isNumber(const std::string& token) const;
    bool isOperator(const std::string& token) const;
    bool isFunction(const std::string& token) const;
    int getPrecedence(const std::string& token) const;
    bool isLeftAssociative(const std::string& token) const;
    std::vector<std::string> tokenize(const std::string& expression) const;
};

#endif
