#include <expr.h>

AssignExpr::AssignExpr(Token name, std::unique_ptr<Expr> value) : m_name(std::move(name)), m_value(std::move(value))
{
	assert(this->m_value != nullptr);
}

llvm::Value *AssignExpr::accept(ExprVisitor &visitor) const
{
	auto &context = get_llvm_cfg_context();
	return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 6969);
}

BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) : m_left(std::move(left)),
																							m_op(op),
																							m_right(std::move(right))
{
	assert(this->m_left != nullptr);
	assert(this->m_right != nullptr);
}

llvm::Value *BinaryExpr::accept(ExprVisitor &visitor) const
{
	auto &context = get_llvm_cfg_context();
	return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 6969);
}

CallExpr::CallExpr(std::unique_ptr<Expr> callee, Token paren,
				   std::vector<std::unique_ptr<Expr>> arguments) : m_callee(std::move(callee)), m_paren(paren), m_arguments(std::move(arguments))
{
	assert(m_paren.get_type() == TokenType::RightParen);
}

llvm::Value *CallExpr::accept(ExprVisitor &visitor) const
{
	auto &context = get_llvm_cfg_context();
	return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 6969);
}

GroupingExpr::GroupingExpr(std::unique_ptr<Expr> expr) : m_expr(std::move(expr))
{
	assert(this->m_expr != nullptr);
}

llvm::Value *GroupingExpr::accept(ExprVisitor &visitor) const
{
	auto &context = get_llvm_cfg_context();
	return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 6969);
}

LiteralExpr::LiteralExpr(std::any literal) : m_literal(std::move(literal))
{
}

llvm::Value *LiteralExpr::accept(ExprVisitor &visitor) const
{
	return visitor.visit_literal_expr(*this);
}

LogicalExpr::LogicalExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) : m_left(std::move(left)), m_op(op), m_right(std::move(right))
{
	assert(this->m_left != nullptr);
	assert(this->m_right != nullptr);
	assert(m_op.get_type() == TokenType::Or || m_op.get_type() == TokenType::And);
}

llvm::Value *LogicalExpr::accept(ExprVisitor &visitor) const
{
	auto &context = get_llvm_cfg_context();
	return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 6969);
}

UnaryExpr::UnaryExpr(Token op, std::unique_ptr<Expr> right) : m_op(op), m_right(std::move(right))
{
	assert(this->m_right != nullptr);
}

llvm::Value *UnaryExpr::accept(ExprVisitor &visitor) const
{
	auto &context = get_llvm_cfg_context();
	return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 6969);
}

VarExpr::VarExpr(Token name) : m_name(std::move(name))
{
}

llvm::Value *VarExpr::accept(ExprVisitor &visitor) const
{
	auto &context = get_llvm_cfg_context();
	return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 6969);
}