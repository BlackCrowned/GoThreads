#pragma once
#include "thread_pool.h"
#include "mutex_control.h"
#include "service_locator.h"

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
}
