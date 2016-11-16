#include "../../include/detail/worker_thread.h"

namespace gothreads {
    namespace detail {


        worker_thread::worker_thread() :
            _thread(),
            _task_pool(),
            _scheduler(&_task_pool) {
            
        }

        void worker_thread::schedule_task(task&& new_task) {
            _task_pool.add(std::forward<task>(new_task));
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
