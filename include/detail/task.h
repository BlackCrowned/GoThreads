﻿#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <cassert>
#include "stack.h"
#include "../../include/detail/task_data.h"

namespace gothreads
{
    namespace detail {
        
        enum class task_state {
            empty,
            init,
            waiting,
            running,
            blocking,
            stopped,
            reschedule
        };

        class task {
            typedef size_t IdType;
            
            friend class scheduler;
            
            const size_t _default_stack_size = 1 << 15;
            static IdType _id_counter;
            IdType _id;

            std::function<void()> _function_entry_point;
            task_state _task_state;

            std::unique_ptr<stack> _stack;

            task_data* _task_context;
            task_data* _scheduler_context;

        public:
            task();
            template<class Function, class ...Args> task(Function fn, Args&&... args);
            task(const task& t);
            task(task&& t) noexcept;

            template<class Allocator = std::allocator<stack::Type>> void alloc_stack();

            task_state state() const;

            IdType id() const;

            bool executable() const;

            void exec(task_data* ptr);

            void yield(task_state state);
        private:
            void state(task_state state);
            
            static void _cdecl _entry_point(task* t);
            static void _cdecl _return_point(task* t);

            template <class Allocator = std::allocator<stack::Type>> void _setup_context();
            void _update_context();
        };

    }
}

#include "../../src/detail/task.inl"
