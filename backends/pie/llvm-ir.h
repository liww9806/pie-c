#ifndef __BACKENDS_PIE_LLVM_IR_H__
#define __BACKENDS_PIE_LLVM_IR_H__

namespace llvm {

class ConstantFolder;
class IRBuilderDefaultInserter;
class LLVMContext;
class Module;
class Function;
template <typename FolderTy, typename InserterTy> class IRBuilder;
typedef IRBuilder<ConstantFolder, IRBuilderDefaultInserter> IRDefBuilder;

}

namespace pie {


class PieIrBuilder
{
 public:
    PieIrBuilder();

    void finish();

    void add_print(const char* msg);

    int output(const char* filename);

    llvm::LLVMContext& get_context()
    {
        return context;
    }
    llvm::Module& get_module()
    {
        return module;
    }
    llvm::IRDefBuilder& get_builder()
    {
        return builder;
    }

 private:
    llvm::LLVMContext& context;
    llvm::Module& module;
    llvm::IRDefBuilder& builder;
    llvm::Function *printfFunction = nullptr;
    bool finished = false;
};

}  // end of namespace pie

#endif  // end of __BACKENDS_PIE_LLVM_IR_H__