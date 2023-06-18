#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "llvm_cfg.h"

#include <cassert>
#include <memory>
#include <vector>

class ExprVisitor;

class Expr
{
public:
    virtual ~Expr() = default;
    virtual llvm::Value *accept(ExprVisitor &visitor) const = 0;
};

class AssignExpr : public Expr
{
public:
    AssignExpr(Token name, std::unique_ptr<Expr> value);
    llvm::Value *accept(ExprVisitor &visitor) const override;

    const Token &get_name() const { return m_name; }
    const Expr &get_value() const { return *m_value; }

private:
    Token m_name;
    std::unique_ptr<Expr> m_value;
};

class BinaryExpr : public Expr
{
public:
    BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
    llvm::Value *accept(ExprVisitor &visitor) const override;

    const Expr &get_left_expr() const { return *m_left; }
    const Token &get_op() const { return m_op; }
    const Expr &get_right_expr() const { return *m_right; }

private:
    std::unique_ptr<Expr> m_left;
    Token m_op;
    std::unique_ptr<Expr> m_right;
};

class CallExpr : public Expr
{
public:
    CallExpr(std::unique_ptr<Expr> callee, Token paren,
             std::vector<std::unique_ptr<Expr>> arguments);
    llvm::Value *accept(ExprVisitor &visitor) const override;

    const Expr &get_callee() const { return *m_callee; }
    const Token &get_paren() const { return m_paren; }
    const std::vector<std::unique_ptr<Expr>> &get_arguments() const { return m_arguments; }

private:
    std::unique_ptr<Expr> m_callee;
    Token m_paren;
    std::vector<std::unique_ptr<Expr>> m_arguments;
};

class GroupingExpr : public Expr
{
public:
    GroupingExpr(std::unique_ptr<Expr> expr);
    llvm::Value *accept(ExprVisitor &visitor) const override;

    const Expr &get_expr() const { return *m_expr; }

private:
    std::unique_ptr<Expr> m_expr;
};

class LiteralExpr : public Expr
{
public:
    LiteralExpr(std::any literal);
    llvm::Value *accept(ExprVisitor &visitor) const override;

    const std::any &get_literal() const { return m_literal; }

private:
    std::any m_literal;
};

class LogicalExpr : public Expr
{
public:
    LogicalExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
    llvm::Value *accept(ExprVisitor &visitor) const override;

    const Expr &get_left_expr() const { return *m_left; }
    const Token &get_op() const { return m_op; }
    const Expr &get_right_expr() const { return *m_right; }

private:
    std::unique_ptr<Expr> m_left;
    Token m_op;
    std::unique_ptr<Expr> m_right;
};

class UnaryExpr : public Expr
{
public:
    UnaryExpr(Token op, std::unique_ptr<Expr> right);
    llvm::Value *accept(ExprVisitor &visitor) const override;

    const Token &get_op() const { return m_op; }
    const Expr &get_right_expr() const { return *m_right; }

private:
    Token m_op;
    std::unique_ptr<Expr> m_right;
};

class VarExpr : public Expr
{
public:
    VarExpr(Token name);
    llvm::Value *accept(ExprVisitor &visitor) const override;

    const Token &get_name() const { return m_name; }

private:
    Token m_name;
};

// visitor stuff
class ExprVisitor
{
public:
    ~ExprVisitor() = default;

    // virtual std::any visitAssignExpr(const AssignExpr& expr) = 0;
    // virtual std::any visitBinaryExpr(const BinaryExpr& expr) = 0;
    // virtual std::any visitCallExpr(const CallExpr& expr) = 0;
    // virtual std::any visitGroupingExpr(const GroupingExpr& expr) = 0;
    // virtual std::any visitLogicalExpr(const LogicalExpr& expr) = 0;
    virtual llvm::Value *visit_literal_expr(const LiteralExpr &expr) = 0;
    // virtual std::any visitUnaryExpr(const UnaryExpr& expr) = 0;
    // virtual std::any visitVarExpr(const VarExpr& expr) = 0;
};

#endif // EXPR_H
