//
// Created by lby on 2018/3/11.
//

#include <cassert>
#include "core/controller.h"

namespace ts {

    BaseMemoryController::BaseMemoryController(const MemoryDevice &device)
            : m_device(device) {
        m_allocator = QueryAllocator(device);
        assert(m_allocator != nullptr);
    }

    Memory BaseMemoryController::alloc(size_t size) {
        return Memory(std::make_shared<HardMemory>(m_device, m_allocator, size));
    }
}