#pragma once
#include <atomic>

namespace gothreads {
    
    class mutex {
        std::atomic_flag _locked;
    public:
        mutex();

        void lock();
        bool try_lock();
        void unlock();
    };

}
