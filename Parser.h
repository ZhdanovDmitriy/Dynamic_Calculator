#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <stdexcept>
#include <cctype>

enum class TokenType {
    Number,
    Operator,
    Function,
    LeftParen,
    RightParen
};

struct Token {
    TokenType type;
    std::string value;
};

class Parser {
public:
    Parser() = default;
    ~Parser() = default;

    std::vector<Token> tokenize(const std::string& expr);
    void validate(const std::vector<Token>& tokens);
};

#endif
