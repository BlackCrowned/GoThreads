#include "../../include/detail/worker_thread.h"
#include "../../include/detail/thread_pool.h"

namespace gothreads {
    namespace detail {


        worker_thread::worker_thread() :
            _thread(),
            _task_pool(),
            _scheduler(&_task_pool, &_cv_new_task_m, &_task_pool_m, &_cv_new_task, &_state),
            _state(false)
        {
            generic::delegate<void()> del(this, &worker_thread::_thread_entry);
            _thread = std::move(std::thread(del));
        }

        worker_thread::~worker_thread() {
            _state = true;
            _thread.join();
        }

        void worker_thread::schedule_task(task&& new_task) {
            std::unique_lock<std::mutex> task_pool_lock(_task_pool_m);
            _task_pool.add(std::forward<task>(new_task));
            task_pool_lock.unlock();
            _cv_new_task.notify_one();
        }

        size_t worker_thread::current_tasks() const {
            return _task_pool.size();
        }

        void worker_thread::_thread_entry() {
            _scheduler.init();

            _scheduler.run();

        }
    }
}
