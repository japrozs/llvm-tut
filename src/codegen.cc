#include "codegen.h"

Codegen::Codegen() {}
Codegen::~Codegen() {}

void Codegen::generate(const std::vector<std::unique_ptr<Stmt>> &statements)
{
	for (const auto &stmt : statements)
	{
		stmt->accept(*this);
	}
}

std::any Codegen::visit_var_statement(const VarStmt &stmt)
{
	auto &builder = get_llvm_cfg_builder();
	auto &context = get_llvm_cfg_context();

	llvm::AllocaInst *variable = builder->CreateAlloca(llvm::Type::getInt32Ty(*context), nullptr, "var");
	llvm::Value *val = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 6969);
	builder->CreateStore(val, variable);
	if (stmt.has_initializer())
	{
		std::any value = stmt.get_initializer().accept(*this);
	}
	return nullptr;
}

llvm::Value *Codegen::visit_literal_expr(const LiteralExpr &expr)
{
	// return ConstantFP::get(*TheContext, APFloat(Val));
	auto &context = get_llvm_cfg_context();
	return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), 6969);
}