#include "../../include/detail/scheduler.h"

#include "../../include/detail/task_pool.h"
#include "../../include/detail/message_queue.h"

namespace gothreads {
    namespace detail {

        scheduler::scheduler(task_pool* ptask_pool, mutex_control* pmutex_control, std::shared_ptr<message_queue<size_t>> mq_ptr) :
        _task_pool(ptask_pool),
        _mutex_control(pmutex_control),
        _mq(mq_ptr),
        _task_data()
        {
            
        }

        void scheduler::init() {
            
        }

        void scheduler::run() {
            while(true) {
                if (!_mq.empty()) {
                    auto msg = _mq.receive();
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

                    switch(task.state()) {
                        case task_state::stopped:
                            _task_pool->erase_current_task();
                            break;
                        case task_state::reschedule:
                            task.state(task_state::blocking);
                            _task_pool->erase_current_task();
                            break;
                        default: break;
                    }
                    
                }
                else {
                    _mq.wait();
                }
            }
        }

        size_t scheduler::message_queue_id() const {
            return _mq.id();
        }
    }
}
