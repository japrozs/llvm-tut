#ifndef TOKEN_H
#define TOKEN_H

#include <any>
#include <string>

#include "token_type.h"

class Token {
public:
    Token(TokenType type, std::string lexeme, int line);
    Token(TokenType type, std::string lexeme, std::any literal, int line);

    TokenType get_type() const { return m_type; }
    const std::string& get_text() const { return m_lexeme; }
    const std::any& get_literal() const { return m_literal; }
    int get_line() const { return m_line; }

    std::string to_string() const;
    std::string literal_to_string() const;

private:
    TokenType m_type;
    std::string m_lexeme;
    std::any m_literal;
    int m_line;
};

#endif // TOKEN_H