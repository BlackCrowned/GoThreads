#pragma once
#include <thread>
#include "scheduler.h"
#include "task_pool.h"

namespace gothreads {
    namespace detail {
        
        class worker_thread {
            std::thread _thread;
            task_pool _task_pool;
            scheduler _scheduler;

        public:
            worker_thread();

            void schedule_task(task&& new_task);

            task_pool const& get_task_pool() const;
            task_pool& get_task_pool();

            size_t current_tasks() const;

        private:
            void _thread_entry();
        };

    }
}
