#pragma once
#include <unordered_map>
#include "task.h"
#include "worker_thread.h"
#include "mutex_control.h"

namespace gothreads {
    namespace detail {
        class thread_pool {
        public:
            using IdType = size_t;
            using ThreadIdType = worker_thread::IdType;

        private:
            std::unordered_map<IdType, worker_thread> _worker_threads;
            std::unordered_map<ThreadIdType, IdType> _thread_id_table;

            mutex_control _mutex_control;

            size_t _max_threads;

            IdType _id;

        public:

            thread_pool();
            void schedule_task(task&& new_task);

            void wait_for_mutex(ThreadIdType const& id, mutex const* mutex);
            void yield_task(ThreadIdType const& id, task_state state = task_state::waiting);

            size_t active_threads();
        private:

            worker_thread& _get_worker_thread();
            worker_thread& _allocate_worker_thread();
        };
    }
}
