#pragma once
#include <queue>
#include <unordered_map>
#include <memory>
#include "task.h"
#include "message_queue.h"

namespace gothreads {
    class mutex;
    
    namespace detail {

        class mutex_control_data {
        public:
            using IdType = size_t;
        private:
            IdType _owner;
            std::queue<task> _waitlist;

        public:
            mutex_control_data();
            ~mutex_control_data() = default;

            mutex_control_data(mutex_control_data const& other) = default;
            mutex_control_data(mutex_control_data&& other) noexcept = default;

            void owner(IdType task_id);
            IdType owner() const;

            void add_to_waitlist(task&& t);
            task next_task();

            bool empty() const;
        };

        class mutex_control {
        public:
            using IdType = size_t;
        private:
            std::unordered_map<mutex const*, mutex_control_data> _data;
            std::shared_ptr<detail::message_queue<size_t>> _mq;
        public:
            mutex_control(std::shared_ptr<detail::message_queue<size_t>> mq);
            ~mutex_control() = default;

            mutex_control(mutex_control const& other) = default;
            mutex_control(mutex_control&& other) noexcept = default;

            void lock_task(mutex const* m, IdType task_id);
            void unlock_task(mutex const* m);

            void wait_for_mutex(mutex const* m, task&& t);
        };

    }
}
