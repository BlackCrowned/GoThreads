#include "../../include/detail/scheduler.h"

#include "../../include/detail/task_pool.h"

namespace gothreads {
    namespace detail {

        scheduler::scheduler(task_pool* ptask_pool, std::mutex* cv_new_task_m, std::mutex* task_pool_m, std::condition_variable* cv_new_task, int* state) :
        _task_pool(ptask_pool),
        _cv_new_task_m(cv_new_task_m),
        _task_pool_m(task_pool_m),
        _cv_new_task(cv_new_task),
        _state(state),
        _task_data()
        {
            
        }

        void scheduler::init() {
            
        }

        void scheduler::run() {
            std::unique_lock<std::mutex> task_pool_lk(*_task_pool_m, std::defer_lock);
            while(true) {
                if (*_state == true) {
                    return;
                }
                task_pool_lk.lock();
                if (!_task_pool->empty()) {
                    auto& task = _task_pool->next();
                    task_pool_lk.unlock();

                    if (task.executable()) {
                        task.exec(&_task_data); //TODO Current Context
                    }
                }
                else {
                    task_pool_lk.unlock();
                    std::unique_lock<std::mutex> lk(*_cv_new_task_m);
                    _cv_new_task->wait(lk);
                    lk.unlock();
                }
            }
        }
    }
}