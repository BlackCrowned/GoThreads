#include "../../include/detail/task.h"
#include "../../include/detail/asm/imports.h"

namespace gothreads {
    namespace detail {

        task::task() :
            _function_entry_point(),
            _task_state(task_state::empty),
            _stack(),
            _exec_reroute(this, &task::_exec_setup)
        {
            _task_state = task_state::waiting;
        }
        
        task_state task::state() const {
            return _task_state;
        }

        bool task::executable() const {
            return _task_state == task_state::waiting && _task_state != task_state::empty;
        }

        const task_data* task::exec(const task_data* ptr) {
            _task_state = task_state::running;
            return _exec_reroute(ptr);
        }

        const task_data* task::_exec_setup(const task_data* ptr) {
            //Setup
            _setup_context();

            //Skip init on later calls
            _exec_reroute = _exec_reroute.from(this, &task::_exec_continue);

            //Continue exec
            return _exec_continue(ptr);
        }

        const task_data* task::_exec_continue(const task_data* ptr) {
            assembler::swap_context(_stack->data(), const_cast<task_data*>(ptr));
            //Make sure to return to this
            _task_state = task_state::waiting;
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

        void task::_entry_point() {
            _task_state = task_state::running;

            //Call function
            _function_entry_point();
        }

        void task::_return_point() {
            //Cease and desist
            _task_state = task_state::stopped;
        }

        void task::_setup_context() {
            //Make sure stack exists
            if (!_stack) {
                _stack = stack::create(_default_stack_size);
            }
            
            task_data* _task_data = reinterpret_cast<task_data*>(_stack->data());

            _task_data->m_task = this;
            _task_data->m_context.m_registers.esp = reinterpret_cast<decltype(_task_data->m_context.m_registers)::type>(_stack->data()) + sizeof(task_data);
            _task_data->m_context.m_registers.ebp = _task_data->m_context.m_registers.esp;
            _task_data->m_context.m_registers.eip = reinterpret_cast<decltype(_task_data->m_context.m_registers)::type>(        //Points to _function_entry_point
                _function_entry_point.target<decltype(_task_data->m_context.m_registers)::type>());
        }
    }
}
