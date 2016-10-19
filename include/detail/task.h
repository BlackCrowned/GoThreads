#pragma once
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
            const int _default_stack_size = 2^10;


            std::function<void()> _function_entry_point;
            task_state _task_state;

            std::unique_ptr<stack> _stack;

           generic::delegate<task_data const* (const task_data*)> _exec_reroute;

        public:
            task();
            template<class Function, class ...Args> task(Function fn, Args&&... args);

            template<class Allocator = std::allocator<stack::Type>> void alloc_stack();

            task_state state() const;

            bool executable() const;

            const task_data* exec(const task_data* ptr);

        private:
            const task_data* _exec_setup(const task_data* ptr);
            const task_data* _exec_continue(const task_data* ptr);
            void _exec_finish(const task_data* ptr);

            void _entry_point();
            void _return_point();

            void _setup_context();
        };

    }
}

#include "../../src/detail/task.inl"
