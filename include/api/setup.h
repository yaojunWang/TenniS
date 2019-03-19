//
// Created by keir on 2019/3/16.
//

#ifndef TENSORSTACK_API_SETUP_H
#define TENSORSTACK_API_SETUP_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @return True if succeed.
 * @note Happen nothing if failed.
 */
TENSOR_STACK_C_API ts_bool ts_setup();

#ifdef __cplusplus
}
#endif

#endif //TENSORSTACK_API_SETUP_H