#include "../../include/detail/scheduler.h"

#include "../../include/detail/task_pool.h"

namespace gothreads {
    namespace detail {

        scheduler::scheduler(task_pool* ptask_pool, std::mutex* cv_new_task_m, std::mutex* task_pool_m, std::condition_variable* cv_new_task, message_queue* receiver_queue, message_queue* sender_queue) :
        _task_pool(ptask_pool),
        _cv_new_task_m(cv_new_task_m),
        _task_pool_m(task_pool_m),
        _cv_new_task(cv_new_task),
        _receiver_queue(receiver_queue),
        _sender_queue(sender_queue),
        _task_data()
        {
            
        }

        void scheduler::init() {
            
        }

        void scheduler::run() {
            std::unique_lock<std::mutex> task_pool_lk(*_task_pool_m, std::defer_lock);
            while(true) {
                if (!_receiver_queue->empty()) {
                    auto msg = _receiver_queue->receive();
                    if (msg->type() == typeid(messages::exit_thread)) {
                        auto m = static_cast<messages::exit_thread*>(msg.get());
                        if (m->force() == true)
                            return;
                    }
                }
                task_pool_lk.lock();
                if (!_task_pool->empty()) {
                    auto& task = _task_pool->next();
                    task_pool_lk.unlock();

                    if (task.executable()) {
                        task.exec(&_task_data); //TODO Current Context
                    }

                    if (task.state() == task_state::stopped)
                    {
                        task_pool_lk.lock();
                        _task_pool->erase_current_task();
                        task_pool_lk.unlock();
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
