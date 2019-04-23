//
// Created by kier on 2019-04-13.
//

#include <core/tensor_builder.h>
#include "frontend/intime.h"
#include "runtime/operator.h"
#include "global/operator_factory.h"

#include "runtime/stack.h"

#include "utils/ctxmgr_lite.h"

namespace ts {
    namespace intime {
        Tensor run(Workbench &bench, const Bubble &bubble, const std::vector<Tensor> &inputs) {
            bench.online_run(bubble, inputs);
            auto &stack = bench.stack();
            auto fields_count = stack.size();
            if (fields_count == 1) {
                return stack[0];
            }
            std::vector<Tensor> fields(fields_count);
            for (int i = 0; i < fields_count; ++i) {
                fields[i] = stack[i];
            }
            Tensor output;
            output.pack(fields);
            return std::move(output);
        }

        Tensor run(const Bubble &bubble, const std::vector<Tensor> &inputs) {
            auto bench = ctx::get<Workbench>();
            if (bench == nullptr) {
                TS_LOG_ERROR << "Must bind Workbench before run" << eject;
            }
            return run(*bench, bubble, inputs);
        }

        Tensor resize2d(const Tensor &x, const Tensor &size, desc::ResizeType type) {
            return run(desc::resize2d(type), {x, size});
        }

        Tensor resize2d(const Tensor &x, const std::vector<int32_t> &size, desc::ResizeType type) {
            return resize2d(x, tensor::from(size), type);
        }

        Tensor add(const Tensor &lhs, const Tensor &rhs) {
            return run(desc::add(), {lhs, rhs});
        }

        Tensor sub(const Tensor &lhs, const Tensor &rhs) {
            return run(desc::sub(), {lhs, rhs});
        }

        Tensor mul(const Tensor &lhs, const Tensor &rhs) {
            return run(desc::mul(), {lhs, rhs});
        }

        Tensor div(const Tensor &lhs, const Tensor &rhs) {
            return run(desc::div(), {lhs, rhs});
        }
    }
}