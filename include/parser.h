#ifndef PARSER_H
#define PARSER_H

#include "expr.h"
#include "statement.h"
#include "token.h"
#include "token_type.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

class Parser {
public:
    Parser(std::vector<Token> tokens);
    std::vector<std::unique_ptr<Stmt>> parse();

private:
    bool check(TokenType type) const;

    bool match(TokenType type);

    Token advance();
    Token peek() const;
    Token previous() const;
    bool is_at_end() const;

    void synchronize();
    Token consume(TokenType type, const char* message);

    class ParseError : public std::runtime_error {
    public:
        ParseError()
            : std::runtime_error("")
        {
        }
    };

    ParseError error(Token token, const char* message) const;

    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> var_declaration();
    std::unique_ptr<Stmt> function();

    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> for_statement();
    std::unique_ptr<Stmt> if_statement();
    std::unique_ptr<Stmt> print_statement();
    std::unique_ptr<Stmt> return_statement();
    std::unique_ptr<Stmt> while_statement();
    std::vector<std::unique_ptr<Stmt>> block();
    std::unique_ptr<Stmt> expression_statement();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> or_expression();
    std::unique_ptr<Expr> and_expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> addition();
    std::unique_ptr<Expr> multiplication();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> call();
    std::unique_ptr<Expr> finish_call(std::unique_ptr<Expr> callee);
    std::unique_ptr<Expr> primary();

    // data
    std::vector<Token> m_tokens;
    int m_current { 0 };
};

#endif // PARSER_H
