#pragma once
#include <thread>
#include "scheduler.h"
#include "task_pool.h"
#include "message_queue.h"
#include "../../dependencies/SRDelegate.hpp"

namespace gothreads {
    class mutex;

    namespace detail {
        class worker_thread {
        public:
            using IdType = decltype(std::declval<std::thread>().get_id());

        private:
            std::thread _thread;
            task_pool _task_pool;
            scheduler _scheduler;

            message_queue _sender_queue;
            message_queue _receiver_queue;

        public:
            worker_thread();
            worker_thread(worker_thread const& wt) = delete;
            worker_thread(worker_thread&& wt) noexcept = delete;

            ~worker_thread();

            worker_thread& operator=(worker_thread&& wt) noexcept;

            void schedule_task(task&& new_task);
            void yield_task(task_state state) const;

            void wait_for_mutex(mutex const* mutex);

            void reschedule_task();

            task_pool const& get_task_pool() const;
            task_pool& get_task_pool();

            size_t current_tasks() const;

            IdType id() const;

        private:
            void _thread_entry();
        };

    }
}
