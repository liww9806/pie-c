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
    // 创建一个函数，返回类型为void
    llvm::FunctionType *functionType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
    llvm::Function *helloFunction = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "main", &module);
 
    // 创建基本块
    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context, "entry", helloFunction);
    builder.SetInsertPoint(entryBlock);
 
    // 打印"Hello World!"
    functionType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
    printfFunction = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "printf", &module);
    builder.CreateCall(printfFunction, builder.CreateGlobalStringPtr("Hello pie!\n"));
}

void PieIrBuilder::finish()
{
    // 返回
    finished = true;
    builder.CreateRetVoid();
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
        // 处理错误信息
        return -1;
    }
    // 写入IR到文件
    module.print(Out, nullptr);
    Out.close();
    return 0;
}

}  // end of namespace pie
