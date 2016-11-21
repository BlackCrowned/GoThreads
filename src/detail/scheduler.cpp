#include "../../include/detail/scheduler.h"

#include "../../include/detail/task_pool.h"

namespace gothreads {
    namespace detail {

        scheduler::scheduler(task_pool* ptask_pool, message_queue* receiver_queue, message_queue* sender_queue) :
        _task_pool(ptask_pool),
        _receiver_queue(receiver_queue),
        _sender_queue(sender_queue),
        _task_data()
        {
            
        }

        void scheduler::init() {
            
        }

        void scheduler::run() {
            while(true) {
                if (!_receiver_queue->empty()) {
                    auto msg = _receiver_queue->receive();
                    if (msg->type() == typeid(messages::exit_thread)) {
                        auto m = static_cast<messages::exit_thread*>(msg.get());
                        if (m->force() == true)
                            return;
                    }
                    else if (msg->type() == typeid(messages::add_task)) {
                        auto m = static_cast<messages::add_task*>(msg.get());
                        _task_pool->add(m->get());
                    }
                }
                if (!_task_pool->empty()) {
                    auto& task = _task_pool->next();

                    if (task.executable()) {
                        task.exec(&_task_data); //TODO Current Context
                    }

                    if (task.state() == task_state::stopped)
                    {
                        _task_pool->erase_current_task();
                    }
                    
                }
                else {
                    _receiver_queue->wait();
                }
            }
        }
    }
}
