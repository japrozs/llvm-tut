#include "llvm_cfg.h"

std::unique_ptr<llvm::LLVMContext> context;
std::unique_ptr<llvm::IRBuilder<>> builder;
std::unique_ptr<llvm::Module> llvm_module;
std::unique_ptr<llvm::legacy::FunctionPassManager> fpm;
std::unique_ptr<llvm::ExitOnError> exit_on_error;

std::unique_ptr<llvm::LLVMContext> &get_llvm_cfg_context()
{
	return context;
}

std::unique_ptr<llvm::IRBuilder<>> &get_llvm_cfg_builder()
{
	return builder;
}

std::unique_ptr<llvm::Module> &get_llvm_cfg_module()
{
	return llvm_module;
}

std::unique_ptr<llvm::legacy::FunctionPassManager> &get_llvm_cfg_fpm()
{
	return fpm;
}

void init_llvm_cfg()
{
	context = std::make_unique<llvm::LLVMContext>();
	llvm_module = std::make_unique<llvm::Module>("vexlang", *context);
	builder = std::make_unique<llvm::IRBuilder<>>(*context);

	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();

	std::string target_triple = llvm::sys::getDefaultTargetTriple(); // x86_64-apple-darwin19.6.0 on my machine
	llvm_module->setTargetTriple(target_triple);

	std::string err;
	auto target = llvm::TargetRegistry::lookupTarget(target_triple, err);

	if (!target)
	{
		std::cout << "err : " << err << "\n";
		exit(1);
	}

	std::string cpu = "generic";
	std::string features = "";

	llvm::TargetOptions opts;
	auto rm = llvm::Optional<llvm::Reloc::Model>();
	auto target_machine = target->createTargetMachine(target_triple, cpu, features, opts, rm);
	llvm_module->setDataLayout(target_machine->createDataLayout());

	//   fpm = std::make_unique<llvm::legacy::FunctionPassManager>(llvm_module.get());

	llvm::Function *main_fn = llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false), llvm::Function::ExternalLinkage, "main", *llvm_module);
	llvm::BasicBlock *entry = llvm::BasicBlock::Create(*context, "entry", main_fn);
	builder->SetInsertPoint(entry);
}