#ifndef __BACKENDS_PIE_PARSER_H__
#define __BACKENDS_PIE_PARSER_H__

#include "ir/ir.h"
#include "lib/json.h"
#include "frontends/p4/typeMap.h"
#include "frontends/common/resolveReferences/referenceMap.h"

namespace pie {

class PieParser : public Inspector
{
 public:
    llvm::Function *printfFunction = nullptr;
    PieParser(llvm::LLVMContext& context,
        llvm::Module& module,
        llvm::IRBuilder<>& builder)
        : context(context)
        , module(module)
        , builder(builder) {}

    bool preorder(const IR::ParserState* state) override
    {
        auto name = state->name.name.c_str();
        printf("visit state: %s\n", name);
        cstring msg("    the parser found a state: ");
        msg += name;
        msg += "\n";
        builder.CreateCall(printfFunction, builder.CreateGlobalStringPtr(msg.c_str()));
        return false;
    }
 private:
    llvm::LLVMContext& context;
    llvm::Module& module;
    llvm::IRBuilder<>& builder;
    P4::ReferenceMap*    refMap = nullptr;
    P4::TypeMap*         typeMap = nullptr;
};

}  // end of namespace pie

#endif  // end of __BACKENDS_PIE_PARSER_H__