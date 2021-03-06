//
// Created by kier on 2019/2/16.
//

#ifndef TENSORSTACK_BACKEND_BASE_BASE_CONV2D_H
#define TENSORSTACK_BACKEND_BASE_BASE_CONV2D_H

#include "operator_on_device.h"
#include <valarray>

#include "backend/common_structure.h"
#include "base_conv2d_core.h"

namespace ts {
    namespace base {
        class Conv2D : public OperatorOnDevice, public Conv2DCore {
        public:
            using self = Conv2D;
            using supper = OperatorOnDevice;

            Conv2D();

            void init() override;

            /**
             *
             * @param stack Contains x, w
             * @return 1
             */
            int run(Stack &stack) override;

            int infer(Stack &stack, std::vector<Tensor::Prototype> &output) override;

        private:
            Conv2DFormat m_format;
            std::valarray<int> m_padding4x2;
            float m_padding_value;
            std::valarray<int> m_stride4;
            std::valarray<int> m_dilation4;

            bool m_kernel_packed = false;
        };
    }
}


#endif //TENSORSTACK_BACKEND_BASE_BASE_CONV2D_H
