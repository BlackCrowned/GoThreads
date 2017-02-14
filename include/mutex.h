#pragma once
#include <atomic>
#include "detail/base.h"

namespace gothreads {
    
    class mutex : detail::base{
        std::atomic_flag _locked;
    public:
        mutex();

        void lock();
        bool try_lock();
        void unlock();
    };

}
