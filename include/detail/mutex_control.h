#pragma once
#include <queue>
#include <unordered_map>
#include <memory>

namespace gothreads {
    class mutex;
    
    namespace detail {

        class mutex_control_data {
        public:
            using IdType = size_t;
        private:
            IdType _task;
            std::queue<IdType> _waitlist;
        public:
            mutex_control_data();
            ~mutex_control_data() = default;

            mutex_control_data(mutex_control_data const& other) = default;
            mutex_control_data(mutex_control_data&& other) noexcept = default;

            void task(IdType task_id);
            IdType task() const;

            void add_to_waitlist(IdType task_id);
            IdType next_task();

            size_t empty() const;
        };

        class mutex_control {
        public:
            using IdType = size_t;
        private:
            std::unordered_map<mutex const*, mutex_control_data> _data;
        public:
            mutex_control();
            ~mutex_control() = default;

            mutex_control(mutex_control const& other) = default;
            mutex_control(mutex_control&& other) noexcept = default;

            void lock_task(mutex const* m, IdType task_id);
            void unlock_task(mutex const* m);

            void wait_for_mutex(mutex const* m, IdType task_id);
        };

    }
}
