//
// Created by kier on 2018/5/25.
//

#ifndef TENSORSTACK_RUNTIME_WORKBENCH_H
#define TENSORSTACK_RUNTIME_WORKBENCH_H

#include <memory>
#include <queue>
#include <unordered_map>
#include <core/device_context.h>

#include "operator.h"
#include "stack.h"
#include "core/tensor.h"
#include "instruction.h"
#include "module/module.h"
#include "inside/thread_pool.h"
#include "global/device_admin.h"
#include "runtime/runtime.h"
#include "image_filter.h"

namespace ts {
    class Workbench {
    public:
        using self = Workbench;    ///< self class
        using shared = std::shared_ptr<self>;  ///< smart pointer

        template<typename K, typename V>
        using map = std::unordered_map<K, V>;

        explicit Workbench(const ComputingDevice &device);

        explicit Workbench(const ComputingDevice &device, int computing_thread_number);

        ~Workbench();

        Workbench(const self &) = delete;

        Workbench &operator=(const self &) = delete;

        Stack &stack() { return *this->m_stack; }

        const Stack &stack() const { return *this->m_stack; }

        void jump_relative(int shift);

        void jump_absolute(size_t pointer);

        void push_data_sagment(int data_index);

        // clear all stack
        void clear() { this->m_stack->clear(); }

        // bind image filter
        void bind_filter(const std::string &name, ImageFilter::shared filter);

        void bind_filter(int slot, ImageFilter::shared filter);

        // set input
        void input(const std::string &name, const Tensor &tensor);

        void input(int slot, const Tensor &tensor);

        const Tensor &input(const std::string &name) const;

        const Tensor &input(int slot) const;

        // run graph
        void run();

        // get output
        const Tensor &output(const std::string &name) const;

        const Tensor &output(int slot) const;

        // clone an Workbench which can run
        Workbench::shared clone() const;

        static shared Load(const Module::shared &module, const ComputingDevice &device);

        const DeviceContext &device() const { return m_device_context; }

        const RuntimeContext &runtime() const { return m_runtime_context; }

        RuntimeContext &runtime() { return m_runtime_context; }

    private:
        size_t m_pointer = 0;   // pointer to running function
        std::vector<Instruction::shared> m_program; // running function, program area
        SyncMemoryController::shared m_static_memory;
        SyncMemoryController::shared m_flow_memory;
        SyncMemoryController::shared m_dynamic_memory;
        Stack::shared m_stack;  // save running memory, data area
        Stack::shared m_data_sagment;   // save static area
        // map slot, means <tensor'name, tensor's index in stack>
        map<std::string, int> m_map_input_slots;
        map<std::string, int> m_map_output_slots;
        // map tensor, means <tensor's index in stack, tensor>
        std::vector<Tensor> m_inputs;
        std::vector<Tensor> m_outputs;

        std::vector<ImageFilter::shared> m_input_filters;

        // control device context
        DeviceContext m_device_context;

        // runtime setting, shared in working thread
        RuntimeContext m_runtime_context;
    };
}


#endif //TENSORSTACK_RUNTIME_WORKBENCH_H
