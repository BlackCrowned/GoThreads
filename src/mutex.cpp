#include "../include/mutex.h"
#include "../include/gothreads.h"

namespace gothreads {

    mutex::mutex() : base(),
    _locked()
    {
        _locked.clear(std::memory_order_release);
    }

    void mutex::lock() {
        if (_locked.test_and_set(std::memory_order_acquire)) {
            //TODO Yield to scheduler
        }
    }

    bool mutex::try_lock() {
        return !_locked.test_and_set(std::memory_order_acquire);
    }

    void mutex::unlock() {
        _locked.clear(std::memory_order_release);
    }
}
