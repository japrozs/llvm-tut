#include <statement.h>

BlockStmt::BlockStmt(std::vector<std::unique_ptr<Stmt>> statements)
    : m_statements(std::move(statements))
{
}

std::any BlockStmt::accept(StmtVisitor &visitor) const
{
    // TODO: fix this
    return nullptr;
}

ExpressionStmt::ExpressionStmt(std::unique_ptr<Expr> expr)
    : m_expr(std::move(expr))
{
    assert(this->m_expr != nullptr);
}

std::any ExpressionStmt::accept(StmtVisitor &visitor) const
{
    // TODO: fix this
    return nullptr;
}

FunctionStmt::FunctionStmt(Token name, std::vector<Token> params,
                           std::vector<std::unique_ptr<Stmt>> body)
    : m_name(name), m_params(std::move(params)), m_body(std::move(body))
{
    assert(m_name.get_type() == TokenType::Identifier);
}

std::any FunctionStmt::accept(StmtVisitor &visitor) const
{
    // TODO: fix this
    return nullptr;
}

IfStmt::IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> then_branch,
               std::unique_ptr<Stmt> else_branch)
    : m_condition(std::move(condition)), m_then_branch(std::move(then_branch)), m_else_branch(std::move(else_branch))
{
    assert(this->m_condition != nullptr);
    assert(this->m_then_branch != nullptr);
    // else_branch is optional
}

std::any IfStmt::accept(StmtVisitor &visitor) const
{
    // TODO: fix this
    return nullptr;
}

PrintStmt::PrintStmt(std::unique_ptr<Expr> expr)
    : m_expr(std::move(expr))
{
    assert(this->m_expr != nullptr);
}

std::any PrintStmt::accept(StmtVisitor &visitor) const
{
    // TODO: fix this
    return nullptr;
}

ReturnStmt::ReturnStmt(Token keyword, std::unique_ptr<Expr> value)
    : m_keyword(std::move(keyword)), m_value(std::move(value))
{
    assert(this->m_keyword.get_type() == TokenType::Return);
}

std::any ReturnStmt::accept(StmtVisitor &visitor) const
{
    // TODO: fix this
    return nullptr;
}

VarStmt::VarStmt(Token name, std::unique_ptr<Expr> expr)
    : m_name(std::move(name)), m_expr(std::move(expr))
{
    assert(this->m_name.get_type() == TokenType::Identifier);
}

std::any VarStmt::accept(StmtVisitor &visitor) const
{
    return visitor.visit_var_statement(*this);
}

WhileStmt::WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
    : m_condition(std::move(condition)), m_body(std::move(body))
{
    assert(this->m_condition != nullptr);
    assert(this->m_body != nullptr);
}

std::any WhileStmt::accept(StmtVisitor &visitor) const
{
    // TODO: fix th
    return nullptr;
}