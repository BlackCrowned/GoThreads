#pragma once
#include "thread_pool.h"
#include <memory>

namespace gothreads {
    namespace detail {
        class service_locator {
            mutable std::weak_ptr<thread_pool> _thread_pool;
        public:

            std::shared_ptr<thread_pool> get_thread_pool() const;

        };
    }
}
