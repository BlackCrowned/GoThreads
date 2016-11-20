#include "../../include/detail/task_data.h"

namespace gothreads {
    namespace detail {

        template<class Function, class ...Args>
        task::task(Function fn, Args&&... args) :
        _function_entry_point(std::bind(fn, std::forward<Args>(args)...)),
        _task_state(task_state::empty),
        _stack(),
        _scheduler_context(nullptr)
        {
            _task_state = task_state::waiting;
            _setup_context();
        }

        template <class Allocator>
        void task::alloc_stack() {
            _stack = stack::create<Allocator>(_default_stack_size);
        }

        template <class Allocator>
        void task::_setup_context() {
            //Make sure stack exists
            if (!_stack) {
                _stack = stack::create<Allocator>(_default_stack_size);
            }

            task_data* _task_data = reinterpret_cast<task_data*>(_stack->data());

            _task_data->m_task = this;
            _task_data->m_context.m_registers.esp = reinterpret_cast<decltype(_task_data->m_context.m_registers)::type>(_stack->data()) + sizeof(stack::Type) * _stack->size();
            _task_data->m_context.m_registers.ebp = _task_data->m_context.m_registers.esp;
            _task_data->m_context.m_registers.eip = reinterpret_cast<decltype(_task_data->m_context.m_registers)::type>(&_entry_point);
            _task_data->m_context.m_registers.ret = reinterpret_cast<decltype(_task_data->m_context.m_registers)::type>(&_return_point);
        }

    }
}
