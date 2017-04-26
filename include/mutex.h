#pragma once
#include <atomic>
#include "detail/base.h"

namespace gothreads {
    
    namespace detail
    {
        class mutex_data {
            bool _locked;
            size_t _owner;
            std::queue<detail::task> _queue;

        public:
            explicit mutex_data();
            ~mutex_data();

            void locked(bool l);
            bool locked() const;

            void owner(size_t id);
            size_t owner() const;

            void push_task(detail::task&& t);
            detail::task pop_task();

            size_t size() const;
            bool empty() const;
        };
    }

    class mutex : detail::base{
        std::mutex _m;
        detail::mutex_data _mutex_data;
    public:
        mutex();

        void lock();
        bool try_lock();
        void unlock();

    private:
        void _wait_for_mutex(std::unique_lock<std::mutex>& lock, detail::mutex_data& data) const;
    };

}
