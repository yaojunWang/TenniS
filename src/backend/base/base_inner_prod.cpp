//
// Created by kier on 2019/2/18.
//

#include <backend/base/base_inner_prod.h>

#include "backend/base/base_inner_prod.h"

namespace ts {
    namespace base {

        void InnerProd::init() {
            supper::init();
        }

        int InnerProd::infer(Stack &stack, std::vector<Tensor::Prototype> &output) {
            TS_AUTO_CHECK(stack.size() == 2);

            auto &lhs = stack[0];
            auto &rhs = stack[1];

            TS_AUTO_CHECK(lhs.dims() == 2 && rhs.dims() == 2 && lhs.size(1) == rhs.size(0));

            TS_AUTO_CHECK(lhs.dtype() == rhs.dtype());

            output.resize(1);
            output[0] = Tensor::Prototype(lhs.dtype(), {lhs.size(0), rhs.size(1)});

            return 1;
        }

        int InnerProd::run(Stack &stack) {
            std::vector<Tensor::Prototype> output;
            infer(stack, output);

            auto memory_device = running_memory_device();

            auto lhs = stack[0].view(memory_device);
            auto rhs = stack[1].view(memory_device);

            auto &out = *stack.push(output[0], memory_device);

            inner_prod(lhs, rhs, out);

            return 1;
        }
    }
}