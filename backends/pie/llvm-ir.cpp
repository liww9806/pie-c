#include "llvm-ir.h"

#pragma GCC diagnostic ignored "-Wunused-parameter" 
#pragma GCC diagnostic ignored "-Wunused-function" 
#pragma GCC diagnostic ignored "-Wunused-variable" 
#pragma GCC diagnostic ignored "-Wunused-label" 
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

#include <llvm/IR/IRBuilder.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/TargetSelect.h>
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"

namespace pie {

PieIrBuilder::PieIrBuilder()
    : context(*new llvm::LLVMContext)
    , module(*new llvm::Module("pie-module", context))
    , builder(*new llvm::IRBuilder<>(context))
{
    // create a funtion, with void return type
    llvm::FunctionType *functionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
    llvm::Function *helloFunction = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "main", &module);
 
    // create a basic block
    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context, "entry", helloFunction);
    builder.SetInsertPoint(entryBlock);
 
    // print "Hello World!"
    functionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
    printfFunction = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "printf", &module);
    builder.CreateCall(printfFunction, builder.CreateGlobalStringPtr("Hello pie!\n"));
}

void PieIrBuilder::finish()
{
    // return
    finished = true;
    builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0));
}

void PieIrBuilder::add_print(const char* msg)
{
    assert(!finished);
    builder.CreateCall(printfFunction, builder.CreateGlobalStringPtr(msg));
}

int PieIrBuilder::output(const char* filename)
{
    assert(finished);
    std::error_code ErrorInfo;
    llvm::raw_fd_ostream Out(filename, ErrorInfo, llvm::sys::fs::OF_None);
    if (ErrorInfo) {
        return -1;
    }
    // write to IR file
    module.print(Out, nullptr);
    Out.close();
    return 0;
}

}  // end of namespace pie
