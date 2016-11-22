#pragma once
#include <unordered_map>
#include "task.h"
#include "worker_thread.h"

namespace gothreads {
    namespace detail {
        class thread_pool {
            using IdType = size_t;
            using ThreadIdType = worker_thread::IdType;

            std::unordered_map<IdType, worker_thread> _worker_threads;
            std::unordered_map<ThreadIdType, IdType> _thread_id_table;

            size_t _max_threads;

            IdType _id;

        public:

            thread_pool();
            void schedule_task(task&& new_task);
            void yield_task(ThreadIdType const& id);

            size_t active_threads();
        private:

            worker_thread& _get_worker_thread();
            worker_thread& _allocate_worker_thread();
        };
    }
}
