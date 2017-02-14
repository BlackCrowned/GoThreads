#pragma once
#include "thread_pool.h"
#include "mutex_control.h"
#include <memory>

namespace gothreads {
    namespace detail {
        class service_locator {
            mutable std::shared_ptr<thread_pool> _thread_pool;
            mutable std::shared_ptr<mutex_control> _mutex_control;
        public:

            std::shared_ptr<thread_pool> get_thread_pool() const;
            std::shared_ptr<mutex_control> get_mutex_control() const;

        };
    }
}
