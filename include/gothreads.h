#pragma once
#include <memory>


#include "detail/thread_pool.h"
#include "detail/service_locator.h"

namespace gothreads {
    class go {
        static detail::service_locator _service_locator;
        std::shared_ptr<detail::thread_pool> _thread_pool;

    public:
        template <class FuncT, class ...Args>
        go(FuncT function, Args&&... arguments);


    };

}


#include "../src/gothreads.inl"
