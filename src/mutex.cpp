#include "../include/mutex.h"
#include "../include/gothreads.h"

namespace gothreads {


    mutex::mutex() :
    _locked(ATOMIC_FLAG_INIT)
    {
        
    }

    void mutex::lock() {
        if (_locked.test_and_set(std::memory_order_acquire)) {
            yield();
        }
    }

    bool mutex::try_lock() {
        return !_locked.test_and_set(std::memory_order_acquire);
    }

    void mutex::unlock() {
        _locked.clear(std::memory_order_release);
    }
}
