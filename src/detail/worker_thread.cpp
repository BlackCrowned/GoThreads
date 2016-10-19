#include "../../include/detail/worker_thread.h"

namespace gothreads {
    namespace detail {


        worker_thread::worker_thread() :
            _thread(),
            _task_pool(),
            _scheduler(&_task_pool) {
            
        }

        void worker_thread::_thread_entry() {
            _scheduler.init();

            _scheduler.run();

        }
    }
}
