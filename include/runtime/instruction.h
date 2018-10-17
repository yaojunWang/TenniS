//
// Created by seeta on 2018/6/28.
//

#ifndef TENSORSTACK_RUNTIME_INSTRUCTION_H
#define TENSORSTACK_RUNTIME_INSTRUCTION_H

#include <memory>
#include "operator.h"

namespace ts {

    class Workbench;

    class Instruction {
    public:
        using self = Instruction;    ///< self class
        using shared = std::shared_ptr<self>;  ///< smart pointer

        virtual ~Instruction() = default;

        virtual void run(Workbench &workbench) = 0;
    };

    class LambdaInstruction : public Instruction {
    public:
        using self = LambdaInstruction;    ///< self class
        using shared = std::shared_ptr<self>;  ///< smart pointer
        using supper = Instruction;

        using Lambda = std::function<void(Workbench &workbench)>;

        LambdaInstruction(const Lambda &lambda);

        void run(Workbench &workbench) final;

    private:
        Lambda m_lambda;
    };

    class StackInstruction : public Instruction {
    public:
        using self = StackInstruction;    ///< self class
        using shared = std::shared_ptr<self>;  ///< smart pointer
        using supper = Instruction;

        void run(Workbench &workbench) final;

        virtual void run(Stack &stack) = 0;
    };

    class OperatorInstruction : public Instruction {
    public:
        using self = OperatorInstruction;    ///< self class
        using shared = std::shared_ptr<self>;  ///< smart pointer
        using supper = Instruction;

        explicit OperatorInstruction(const Operator::shared &func, int nargs, int nresults);

        void run(Workbench &workbench) final ;

    private:
        Operator::shared m_func = nullptr;
        int m_nargs = 0;
        int m_nresults = 0;
    };

}


#endif //TENSORSTACK_RUNTIME_INSTRUCTION_H