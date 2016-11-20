#pragma once
#include <thread>
#include "scheduler.h"
#include "task_pool.h"
#include <mutex>

namespace gothreads {
    namespace detail {
        
        class worker_thread {
            std::thread _thread;
            task_pool _task_pool;
            scheduler _scheduler;

            std::mutex _cv_new_task_m;
            std::mutex _task_pool_m;
            std::condition_variable _cv_new_task;

            message_queue _sender_queue;
            message_queue _receiver_queue;

        public:
            worker_thread();
            ~worker_thread();
            void schedule_task(task&& new_task);

            task_pool const& get_task_pool() const;
            task_pool& get_task_pool();

            size_t current_tasks() const;

        private:
            void _thread_entry();
        };

    }
}
