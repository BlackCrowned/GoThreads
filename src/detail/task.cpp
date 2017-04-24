#include "../../include/detail/task.h"
#include "../../include/detail/asm/imports.h"
#include "../../include/detail/scheduler.h"
#include "../../include/detail/message_queue.h"
#include <iostream>

namespace gothreads {
    namespace detail {

        task::IdType task::_id_counter = 0;

        task::task() :
        _id(++_id_counter),
        _function_entry_point(),
        _task_state(task_state::empty),
        _msg(),
        _stack(),
        _task_context(nullptr),
        _scheduler_context(nullptr)
        {
            //_task_state = task_state::waiting;
        }

        task::task(const task& t) :
        _id(t._id),
        _function_entry_point(t._function_entry_point),
        _task_state(t._task_state),
        _msg(t._msg),
        _stack(stack::copy(t._stack)),
        _task_context(t._task_context),
        _scheduler_context(t._scheduler_context)
        {
            //Resetup context
            _setup_context();
        }

        task::task(task&& t) noexcept :
        _id(std::move(t._id)),
        _function_entry_point(std::move(t._function_entry_point)),
        _task_state(std::move(t._task_state)),
        _msg(std::move(t._msg)),
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

        std::shared_ptr<message> task::msg() {
            return std::move(_msg);
        }

        task::IdType task::id() const {
            return _id;
        }

        bool task::executable() const {
            return _task_state == task_state::waiting;
        }

        void task::exec(task_data* ptr) {
            _task_state = task_state::waiting;
            _scheduler_context = ptr;
            
            assembler::swap_context(_task_context, ptr);
        }

        void task::yield(task_state state) {
            //Set state
            this->state(state);

            assembler::swap_context(_scheduler_context, _task_context);
        }

        void task::yield(std::shared_ptr<message> msg) {
            //Set msg
            this->msg(msg);

            assembler::swap_context(_scheduler_context, _task_context);
        }

        void task::state(task_state state) {
            _task_state = state;
        }

        void task::msg(std::shared_ptr<message> msg) {
            _msg = msg;
        }

        void task::_entry_point(task* t) {
            t->_task_state = task_state::running;

            //Call function
            if (t->_function_entry_point)
                t->_function_entry_point->operator()();
        }

        void task::_return_point(task* t) {
            //Cease and desist
            t->_task_state = task_state::stopped;

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
