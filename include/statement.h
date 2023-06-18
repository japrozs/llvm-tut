#ifndef STATEMENT_H
#define STATEMENT_H

#include "expr.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

class StmtVisitor;

class Stmt
{
public:
    virtual ~Stmt() = default;
    virtual std::any accept(StmtVisitor &visitor) const = 0;
};

class BlockStmt : public Stmt
{
public:
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements);
    std::any accept(StmtVisitor &visitor) const override;

    const std::vector<std::unique_ptr<Stmt>> &get_statements() const { return m_statements; }

private:
    std::vector<std::unique_ptr<Stmt>> m_statements;
};

class ExpressionStmt : public Stmt
{
public:
    ExpressionStmt(std::unique_ptr<Expr> expr);
    std::any accept(StmtVisitor &visitor) const override;

    const Expr &get_expr() const { return *m_expr; }

private:
    std::unique_ptr<Expr> m_expr;
};

class FunctionStmt : public Stmt
{
public:
    FunctionStmt(Token name, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body);
    std::any accept(StmtVisitor &visitor) const override;

    const Token &get_name() const { return m_name; };
    const std::vector<Token> &get_params() const { return m_params; }
    const std::vector<std::unique_ptr<Stmt>> &get_body() const { return m_body; }

private:
    Token m_name;
    std::vector<Token> m_params;
    std::vector<std::unique_ptr<Stmt>> m_body;
};

class IfStmt : public Stmt
{
public:
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch);
    std::any accept(StmtVisitor &visitor) const override;

    const Expr &get_condition() const { return *m_condition; }
    const Stmt &get_then_branch() const { return *m_then_branch; }
    bool has_else_branch() const { return m_else_branch != nullptr; }
    const Stmt &get_else_branch() const { return *m_else_branch; }

private:
    std::unique_ptr<Expr> m_condition;
    std::unique_ptr<Stmt> m_then_branch;
    std::unique_ptr<Stmt> m_else_branch;
};

class PrintStmt : public Stmt
{
public:
    PrintStmt(std::unique_ptr<Expr> expr);
    std::any accept(StmtVisitor &visitor) const override;

    const Expr &get_expr() const { return *m_expr; }

private:
    std::unique_ptr<Expr> m_expr;
};

class ReturnStmt : public Stmt
{
public:
    ReturnStmt(Token keyword, std::unique_ptr<Expr> value);
    std::any accept(StmtVisitor &visitor) const override;

    bool has_value() const { return m_value.get() != nullptr; }
    const Expr &get_value() const { return *m_value; }

private:
    Token m_keyword;
    std::unique_ptr<Expr> m_value;
};

class VarStmt : public Stmt
{
public:
    VarStmt(Token name, std::unique_ptr<Expr> expr);
    std::any accept(StmtVisitor &visitor) const override;

    const Token &get_name() const { return m_name; }

    bool has_initializer() const { return m_expr != nullptr; }
    const Expr &get_initializer() const { return *m_expr; }

private:
    Token m_name;
    std::unique_ptr<Expr> m_expr;
};

class WhileStmt : public Stmt
{
public:
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body);
    std::any accept(StmtVisitor &visitor) const override;

    const Expr &get_condition() const { return *m_condition; }
    const Stmt &get_body() const { return *m_body; }

private:
    std::unique_ptr<Expr> m_condition;
    std::unique_ptr<Stmt> m_body;
};

// visitor stuff
class StmtVisitor
{
public:
    ~StmtVisitor() = default;

    // virtual std::any visitBlockStmt(const BlockStmt& stmt) = 0;
    // virtual std::any visitExpressionStmt(const ExpressionStmt& stmt) = 0;
    // virtual std::any visitIfStmt(const IfStmt& stmt) = 0;
    // virtual std::any visitFunctionStmt(const FunctionStmt& stmt) = 0;
    // virtual std::any visitPrintStmt(const PrintStmt& stmt) = 0;
    // virtual std::any visitReturnStmt(const ReturnStmt& stmt) = 0;
    virtual std::any visit_var_statement(const VarStmt &stmt) = 0;
    // virtual std::any visitWhileStmt(const WhileStmt& stmt) = 0;
};

#endif // STATEMENT_H
