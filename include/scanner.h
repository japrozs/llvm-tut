#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include "token_type.h"

#include <any>
#include <iostream>
#include <unordered_map>
#include <vector>

class Scanner {
public:
    Scanner(std::string source);
    std::vector<Token> scan_tokens();

private:
    bool is_at_end() const;
    char advance();
    bool match(char expected);
    char peek() const;
    char peek_next() const;

    void add_token(TokenType type, std::any literal);
    void add_token(TokenType type);

    void comment();
    void string();
    void number();
    void identifier();
    void scan_token();

    // data
    std::string m_source;
    std::vector<Token> m_tokens;

    std::unordered_map<std::string, TokenType> m_keywords;

    int m_start { 0 };
    int m_current { 0 };
    int m_line { 1 };
};

#endif // SCANNER_H