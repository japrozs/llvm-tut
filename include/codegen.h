#ifndef CODEGEN_H
#define CODEGEN_H

#include "statement.h"
#include "expr.h"
#include "llvm_cfg.h"

#include <vector>
#include <memory>

class Codegen : public ExprVisitor, public StmtVisitor
{
public:
	Codegen();
	~Codegen();

	void generate(const std::vector<std::unique_ptr<Stmt>> &statements);

private:
	// statement visitors
	std::any visit_var_statement(const VarStmt &stmt) override;

	// expression visitors
	llvm::Value *visit_literal_expr(const LiteralExpr &expr) override;
};

#endif // CODEGEN_H