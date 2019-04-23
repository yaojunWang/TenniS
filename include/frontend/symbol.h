//
// Created by kier on 2019/3/27.
//

#ifndef TENSORSTACK_FRONTEND_SYMBOL_H
#define TENSORSTACK_FRONTEND_SYMBOL_H

#include <module/graph.h>
#include <frontend/desc.h>

namespace ts {
    namespace symbol {
        TS_DEBUG_API Node pad(const std::string &name, const Node &x, const Node &padding, float padding_value = 0);

        TS_DEBUG_API Node resize2d(const std::string &name, const Node &x, const Node &size,
                                   desc::ResizeType type = desc::ResizeType::LINEAR);

        TS_DEBUG_API Node add(const std::string &name, const Node &lhs, const Node &rhs);

        TS_DEBUG_API Node sub(const std::string &name, const Node &lhs, const Node &rhs);

        TS_DEBUG_API Node mul(const std::string &name, const Node &lhs, const Node &rhs);

        TS_DEBUG_API Node div(const std::string &name, const Node &lhs, const Node &rhs);
    }
}


#endif //TENSORSTACK_FRONTEND_SYMBOL_H