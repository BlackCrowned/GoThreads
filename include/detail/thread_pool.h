#pragma once
#include <unordered_map>
#include "task.h"
#include "worker_thread.h"

namespace gothreads {
    namespace detail {
        class thread_pool {
            std::unordered_map<int, worker_thread> _worker_threads;
            size_t _id;

            size_t _max_threads;

        public:

            thread_pool();
            void schedule_task(task&& new_task);

            size_t active_threads();
        private:

            worker_thread& _get_worker_thread();
            worker_thread& _allocate_worker_thread();
        };
    }
}
