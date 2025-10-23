#include "Parser.h"
#include <sstream>

std::vector<Token> Parser::tokenize(const std::string& expr) {
    std::vector<Token> tokens;
    std::string buffer;

    auto pushNumber = [&](const std::string& num) {
        if (!num.empty()) {
            tokens.push_back({ TokenType::Number, num });
        }
        };

    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];
        if (std::isspace(c)) continue;

        if (std::isdigit(c) || c == '.') {
            buffer += c;
        }
        else {
            pushNumber(buffer);
            buffer.clear();

            if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
                tokens.push_back({ TokenType::Operator, std::string(1, c) });
            }
            else if (c == '(') {
                tokens.push_back({ TokenType::LeftParen, "(" });
            }
            else if (c == ')') {
                tokens.push_back({ TokenType::RightParen, ")" });
            }
            else if (std::isalpha(c)) {
                std::string func;
                while (i < expr.size() && std::isalpha(expr[i])) {
                    func += expr[i];
                    ++i;
                }
                --i;
                tokens.push_back({ TokenType::Function, func });
            }
            else {
                throw std::runtime_error(std::string("Недопустимый символ: ") + c);
            }
        }
    }
    pushNumber(buffer);

    return tokens;
}

void Parser::validate(const std::vector<Token>& tokens) {
    int balance = 0;
    for (const auto& t : tokens) {
        if (t.type == TokenType::LeftParen) balance++;
        if (t.type == TokenType::RightParen) balance--;
        if (balance < 0) throw std::runtime_error("Несбалансированные скобки");
    }
    if (balance != 0) throw std::runtime_error("Несбалансированные скобки");
}
