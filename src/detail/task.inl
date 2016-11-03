#include "../../include/detail/task_data.h"

namespace gothreads {
    namespace detail {

        template<class Function, class ...Args>
        task::task(Function fn, Args&&... args) :
        _function_entry_point(std::bind(fn, std::forward<Args>(args)...)),
        _task_state(task_state::empty),
        _stack(),
        _exec_reroute(this, &task::_exec_setup)
        {

            _task_state = task_state::waiting;
        }

        template <class Allocator>
        void task::alloc_stack() {
            static_assert(_default_stack_size > sizeof(task_data), "_default_stack_size needs to be creater than task_data!");
            _stack = stack::create<Allocator>(_default_stack_size);
        }

    }
}
