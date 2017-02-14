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
            _mutex_control->wait_for_mutex(this, _thread_pool->current_task_id());
            _thread_pool->yield_task(std::this_thread::get_id(), detail::task_state::reschedule);
        }
        else {
            _mutex_control->lock_task(this, _thread_pool->current_task_id());
        }
    }

    bool mutex::try_lock() {
        return !_locked.test_and_set(std::memory_order_acquire);
    }

    void mutex::unlock() {
        _locked.clear(std::memory_order_release);
        _mutex_control->unlock_task(this);
    }
}
