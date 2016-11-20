﻿#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "stack.h"
#include "../../include/detail/task_data.h"
#include "../../dependencies/SRDelegate.hpp"

namespace gothreads
{
    namespace detail {
        
        enum class task_state {
            empty,
            init,
            waiting,
            running,
            stopped
        };

        class task {
            const size_t _default_stack_size = 4096000;


            std::function<void()> _function_entry_point;
            task_state _task_state;

            std::unique_ptr<stack> _stack;

            task_data const* _scheduler_context;

        public:
            task();
            template<class Function, class ...Args> task(Function fn, Args&&... args);
            task(const task& t);
            task(task&& t) noexcept;

            template<class Allocator = std::allocator<stack::Type>> void alloc_stack();

            task_state state() const;

            bool executable() const;

            const task_data* exec(const task_data* ptr);

        private:
            void _exec_finish(const task_data* ptr);

            static void _cdecl _entry_point(task* t);
            static void _cdecl _return_point(task* t);

            template <class Allocator = std::allocator<stack::Type>> void _setup_context();
        };

    }
}

#include "../../src/detail/task.inl"
