#include "../../include/detail/mutex_control.h"

namespace gothreads {
    namespace detail {

        mutex_control_data::mutex_control_data() :
        _owner(0),                       //TODO Invalid Task ID
        _waitlist()
        {
            
        }

        void mutex_control_data::owner(IdType task_id) {
            _owner = task_id;
        }

        mutex_control_data::IdType mutex_control_data::owner() const {
            return _owner;
        }

        void mutex_control_data::add_to_waitlist(task&& t) {
            _waitlist.push(std::forward<task>(t));
        }

        task mutex_control_data::next_task() {
            auto result = std::move(_waitlist.front());
            _waitlist.pop();

            return result;
        }

        bool mutex_control_data::empty() const {
            return _waitlist.empty();
        }

        mutex_control::mutex_control(std::shared_ptr<detail::message_queue<size_t>> mq) :
        _data(),
        _mq(mq)
        {
            
        }

        void mutex_control::lock_task(mutex const* m, IdType task_id) {
            auto& control_data = _data[m];

            control_data.owner(task_id);
        }

        void mutex_control::unlock_task(mutex const* m) {
            auto& control_data = _data[m];

            control_data.owner(0);       //TODO Invalid Task ID


            if (!control_data.empty()) {
                _mq->broadcast(std::make_shared<messages::unlock_task>(std::move(control_data.next_task())));
            }
            if (control_data.empty()) {
                _data.erase(m);
            }
        }

        void mutex_control::wait_for_mutex(mutex const* m, task&& t) {
            auto& control_data = _data[m];

            control_data.add_to_waitlist(std::forward<task>(t));
        }
    }
}