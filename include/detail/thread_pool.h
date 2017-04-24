#pragma once
#include <unordered_map>
#include "task.h"
#include "worker_thread.h"

namespace gothreads {
    namespace detail {
        class thread_pool {
        public:
            using IdType = size_t;
            using ThreadIdType = worker_thread::IdType;

        private:
            std::unordered_map<IdType, worker_thread> _worker_threads;
            std::unordered_map<ThreadIdType, IdType> _thread_id_table;

            std::shared_ptr<message_queue<size_t>> _mq;

            size_t _max_threads;

            IdType _id;

        public:

            thread_pool();
            void schedule_task(task&& new_task);
            void yield_task(ThreadIdType const& id, task_state state = task_state::waiting);
            void yield_task(ThreadIdType const& id, std::shared_ptr<message> msg);

            size_t active_threads() const;
            void max_threads(size_t n);

            IdType current_task_id() const;
            IdType current_task_id(ThreadIdType const& id) const;

            void send_message(size_t id, std::shared_ptr<message>&& msg) const;
            void broadcast_message(std::shared_ptr<message>&& msg) const;
        private:

            worker_thread& _get_worker_thread();
            worker_thread& _allocate_worker_thread();
        };
    }
}
