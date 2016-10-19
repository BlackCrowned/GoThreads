#include "../../include/detail/scheduler.h"

#include "../../include/detail/task_pool.h"

namespace gothreads {
    namespace detail {

        scheduler::scheduler(task_pool* ptask_pool) :
        _task_pool(ptask_pool)
        {
            
        }

        void scheduler::init() {
            
        }

        void scheduler::run() {
            while(true) {
                if (_task_pool->empty()) {
                    auto& task = _task_pool->next();

                    if (task.executable()) {
                        task.exec(nullptr); //TODO Current Context
                    }
                }
            }
        }
    }
}