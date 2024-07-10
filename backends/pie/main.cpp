#include <stdio.h>
#include <string>
#include <iostream>

#include "ir/ir.h"
#include "lib/gc.h"
#include "lib/log.h"
#include "lib/error.h"
#include "lib/json.h"
#include "lib/exceptions.h"
#include "lib/nullstream.h"
#include "control-plane/p4RuntimeSerializer.h"
#include "frontends/common/applyOptionsPragmas.h"
#include "frontends/common/parseInput.h"
#include "frontends/p4/frontend.h"
#include "frontends/common/model.h"
#include "frontends/p4/coreLibrary.h"
#include "midend/convertEnums.h"

#include "frontends/common/options.h"
#include "frontends/p4/evaluator/evaluator.h"
#include "midend/convertEnums.h"

namespace pie {

class MidEnd : public PassManager {
 public:
    // These will be accurate when the mid-end completes evaluation
    P4::ReferenceMap    refMap;
    P4::TypeMap         typeMap;
    const IR::ToplevelBlock   *toplevel = nullptr;
    P4::ConvertEnums::EnumMapping enumMap;

    // explicit MidEnd(BMV2Options& options);
    explicit MidEnd()
    {
        bool isv1 = true; //options.isv1();
        setName("MidEnd");
        refMap.setIsV1(isv1);  // must be done BEFORE creating passes
        auto evaluator = new P4::EvaluatorPass(&refMap, &typeMap);
        addPasses({
            evaluator,
            new VisitFunctor([this, evaluator]() { toplevel = evaluator->getToplevelBlock(); }),
        });
    }
    const IR::ToplevelBlock* process(const IR::P4Program *&program) {
        program = program->apply(*this);
        return toplevel; }
};


class BMV2Options : public CompilerOptions {
 public:
};

using BMV2Context = P4CContextWithOptions<BMV2Options>;

}  // end of namespace pie

#include "parser.h"


int main(int argc, char** argv) {
    setup_gc_logging();

    pie::PieIrBuilder builder;

    AutoCompileContext autoBMV2Context(new pie::BMV2Context);
    auto& options = pie::BMV2Context::get().options();

    if (options.process(argc, argv) != nullptr)
        options.setInputFile();
    if (::errorCount() > 0)
        return 1;

    // BMV2 is required for compatibility with the previous compiler.
    options.preprocessor_options += " -D__TARGET_BMV2__";
    auto program = P4::parseP4File(options);
    if (program == nullptr || ::errorCount() > 0)
        return 1;
    try {
        P4::P4COptionPragmaParser optionsPragmaParser;
        program->apply(P4::ApplyOptionsPragmas(optionsPragmaParser));

        P4::FrontEnd frontend;
        program = frontend.run(options, program);
    } catch (const Util::P4CExceptionBase &bug) {
        std::cerr << bug.what() << std::endl;
        return 1;
    }
    if (program == nullptr || ::errorCount() > 0)
        return 1;

    const IR::ToplevelBlock* toplevel = nullptr;
    pie::MidEnd midEnd;
    try {
        toplevel = midEnd.process(program);
        if (::errorCount() > 1 || toplevel == nullptr ||
            toplevel->getMain() == nullptr)
            return 1;
    } catch (const Util::P4CExceptionBase &bug) {
        std::cerr << bug.what() << std::endl;
        return 1;
    }
    if (::errorCount() > 0)
        return 1;
    
    pie::PieParser parser(builder);
    toplevel->getProgram()->apply(parser);
    builder.finish();
  
    // delete engine;
    auto ret = builder.output("hello.ll");
    if (ret != 0) {
        fprintf(stderr, "failed to output to '%s'\n", "hello.ll");
        return -1;
    }

    auto cmd = "llc -filetype=obj --relocation-model=pic -o hello.o hello.ll";
    ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "failed to exec '%s'\n", cmd);
        return -1;
    }

    cmd = "gcc hello.o -o hello";
    ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "failed to exec '%s'\n", cmd);
        return -1;
    }

    cmd = "./hello";
    printf("exec the cmd: %s\n", cmd);
    system(cmd);

    return 0;
}
