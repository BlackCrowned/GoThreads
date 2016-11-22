#pragma once
#include <memory>

#include "detail/thread_pool.h"
#include "detail/service_locator.h"
    
namespace gothreads {
    namespace detail {
        class base {
        protected:
            static service_locator _service_locator;
            std::shared_ptr<thread_pool> _thread_pool;

        public:
            base();
        };
    }
    
    class go : public detail::base {
    public:
        template <class FuncT, class ...Args>
        explicit go(FuncT function, Args&&... arguments);


    };

    class yield : public detail::base{
    public:
        yield();
    };
}


#include "../src/gothreads.inl"
