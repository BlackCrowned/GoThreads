#include "../../include/detail/scheduler.h"

#include "../../include/detail/task_pool.h"
#include "../../include/detail/message_queue.h"

namespace gothreads {
    namespace detail {

        scheduler::scheduler(task_pool* ptask_pool, std::shared_ptr<message_queue<size_t>> mq_ptr) :
        _task_pool(ptask_pool),
        _mq(mq_ptr),
        _task_data()
        {
            
        }

        void scheduler::init() {
            
        }

        void scheduler::schedule_tasks() {
            auto& task = _task_pool->next();

            if (task.executable()) {
                task.exec(&_task_data); //TODO Current Context
            }

            auto msg = std::move(task.msg());
            if (msg) {
                if (msg->type() == typeid(messages::wait_for_mutex)) {
                    auto m = static_cast<messages::wait_for_mutex*>(msg.get());
                    auto t = _task_pool->release_current_task();

                    //Set state to waiting
                    t.state(task_state::blocking);
                    m->add_to_mutex_data(std::move(t));
                    return;
                }
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
                    else if (msg->type() == typeid(messages::unlock_task)) {
                        auto m = static_cast<messages::unlock_task*>(msg.get());
                        if (!m->empty()) {
                            auto t = m->get();
                            t.state(task_state::waiting);
                            _task_pool->add(std::move(t));
                        }
                    }
                }
                if (!_task_pool->empty()) {
                    schedule_tasks();
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
