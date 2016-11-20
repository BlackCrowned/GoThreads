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
            _scheduler_context(t._scheduler_context)
        {
            //Resetup context
            _setup_context();
        }

        task::task(task&& t) noexcept :
            _function_entry_point(std::move(t._function_entry_point)),
            _task_state(std::move(t._task_state)),
            _stack(std::move(t._stack)),
            _scheduler_context(std::move(t._scheduler_context))
        {
            //Resetup context
            _setup_context();
        }

        task_state task::state() const {
            return _task_state;
        }

        bool task::executable() const {
            return _task_state == task_state::waiting && _task_state != task_state::empty;
        }

        const task_data* task::exec(const task_data* ptr) {
            _task_state = task_state::waiting;
            _scheduler_context = ptr;
            
            assembler::swap_context(_task_context, const_cast<task_data*>(ptr));
            return reinterpret_cast<task_data*>(_stack->data());
        }

        void task::_exec_finish(const task_data* ptr) {
            auto ptask_data = reinterpret_cast<task_data*>(_stack->data());

            ptask_data->m_context.m_registers.eip = 0;    //Set to return addr! //THIS WILL BE IN swap_context
            
            //Return to scheduler context
            void* current_context = assembler::get_current_context();

            if (!current_context) {
                //Uuuhhhh
            }

            assembler::swap_context(const_cast<task_data*>(ptr), current_context);
        }

        void task::_entry_point(task* t) {
            t->_task_state = task_state::running;

            //Call function
            t->_function_entry_point();
        }

        void task::_return_point(task* t) {
            //Cease and desist
            t->_task_state = task_state::stopped;

            assembler::swap_context(const_cast<task_data*>(t->_scheduler_context), t->_task_context);
        }
    }
}
