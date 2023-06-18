#ifndef LLVM_CFG_H
#define LLVM_CFG_H

#include <memory>
#include <iostream>

// LLVM stuff
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/Optional.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

void init_llvm_cfg();
std::unique_ptr<llvm::LLVMContext> &get_llvm_cfg_context();
std::unique_ptr<llvm::IRBuilder<>> &get_llvm_cfg_builder();
std::unique_ptr<llvm::Module> &get_llvm_cfg_module();
std::unique_ptr<llvm::legacy::FunctionPassManager> &get_llvm_cfg_fpm();

#endif // LLVM_CFG_H