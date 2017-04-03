namespace gothreads {
    namespace detail {

        template<class Function, class ...Args>
        task::task(Function fn, Args&&... args) :
        _function_entry_point(std::make_shared<std::function<void()>>(std::bind(fn, std::forward<Args>(args)...))),
        _task_state(task_state::empty),
        _msg(),
        _stack(),
        _task_context(nullptr),
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
            using Type = decltype(_task_context->m_context.m_registers)::type;
            
            //Make sure stack exists
            if (!_stack) {
                _stack = stack::create<Allocator>(_default_stack_size);
            }

            //Allign task_data
            _task_context = reinterpret_cast<task_data*>(_stack->data());
            size_t stack_size = _stack->size();
            _task_context = static_cast<task_data*>(std::align(16, sizeof(task_data), reinterpret_cast<void*&>(_task_context), stack_size));
            assert(_task_context != nullptr);

            //Allign stack
            Type stack_base = reinterpret_cast<Type>(_stack->data() + _stack->size()) & static_cast<Type>(0xFFFFFFF0);

            //Populate task_context
            _task_context->m_task = this;
            _task_context->m_context.m_registers.esp = stack_base - 4 * sizeof(Type);
            _task_context->m_context.m_registers.ebp = stack_base - 4 * sizeof(Type);
            _task_context->m_context.m_registers.eip = 0;

            //Setup stack
            Type* entry_addr = reinterpret_cast<Type*>(stack_base) - 4;
            Type* return_addr = reinterpret_cast<Type*>(stack_base) - 3;
            Type* argument = reinterpret_cast<Type*>(stack_base) - 2;
            Type* argument_return_point = reinterpret_cast<Type*>(stack_base) - 1;

            *entry_addr = reinterpret_cast<Type>(&_entry_point);
            *return_addr = reinterpret_cast<Type>(&_return_point);
            *argument = reinterpret_cast<Type>(this);
            *argument_return_point = reinterpret_cast<Type>(this);
        }

    }
}
