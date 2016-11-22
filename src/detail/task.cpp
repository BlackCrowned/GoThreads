#include "../../include/detail/task.h"
#include "../../include/detail/asm/imports.h"
#include <iostream>

namespace gothreads {
    namespace detail {

        task::task() :
            _function_entry_point(),
            _task_state(task_state::empty),
            _stack(),
            _task_context(nullptr),
            _scheduler_context(nullptr)
        {
            //_task_state = task_state::waiting;
        }

        task::task(const task& t) :
            _function_entry_point(t._function_entry_point),
            _task_state(t._task_state),
            _stack(stack::copy(t._stack)),
            _task_context(t._task_context),
            _scheduler_context(t._scheduler_context)
        {
            //Resetup context
            _setup_context();
        }

        task::task(task&& t) noexcept :
            _function_entry_point(std::move(t._function_entry_point)),
            _task_state(std::move(t._task_state)),
            _stack(std::move(t._stack)),
            _task_context(std::move(t._task_context)),
            _scheduler_context(std::move(t._scheduler_context))
        {
            //Update context
            _update_context();
        }

        task_state task::state() const {
            return _task_state;
        }

        bool task::executable() const {
            return _task_state == task_state::waiting && _task_state != task_state::empty;
        }

        void task::exec(task_data* ptr) {
            _task_state = task_state::waiting;
            _scheduler_context = ptr;
            
            assembler::swap_context(_task_context, ptr);
        }

        void task::yield() {
            _yield_point(this);
        }

        void task::_entry_point(task* t) {
            t->_task_state = task_state::running;

            //Call function
            t->_function_entry_point();
        }

        void task::_return_point(task* t) {
            //Cease and desist
            t->_task_state = task_state::stopped;

            assembler::swap_context(t->_scheduler_context, t->_task_context);
        }

        void task::_yield_point(task* t) {
            //Set state to waiting
            t->_task_state = task_state::waiting;

            assembler::swap_context(t->_scheduler_context, t->_task_context);
        }

        void task::_update_context() {
            using Type = decltype(_task_context->m_context.m_registers)::type;

            //Allign stack
            Type stack_base = reinterpret_cast<Type>(_stack->data() + _stack->size()) & static_cast<Type>(0xFFFFFFF0);

            //Update this pointer
            _task_context->m_task = this;

            //Update this pointer
            Type* argument = reinterpret_cast<Type*>(stack_base) - 2;
            Type* argument_return_point = reinterpret_cast<Type*>(stack_base) - 1;

            *argument = reinterpret_cast<Type>(this);
            *argument_return_point = reinterpret_cast<Type>(this);
        }
    }
}
