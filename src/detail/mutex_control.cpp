#include "../../include/detail/mutex_control.h"

namespace gothreads {
    namespace detail {

        mutex_control_data::mutex_control_data() :
        _task(0),                       //TODO Invalid Task ID
        _waitlist()
        {
            
        }

        void mutex_control_data::task(IdType task_id) {
            _task = task_id;
        }

        mutex_control_data::IdType mutex_control_data::task() const {
            return _task;
        }

        void mutex_control_data::add_to_waitlist(IdType task_id) {
            _waitlist.push(task_id);
        }

        mutex_control_data::IdType mutex_control_data::next_task() {
            auto result = _waitlist.front();
            _waitlist.pop();

            return result;
        }

        size_t mutex_control_data::empty() const {
            return _waitlist.empty();
        }

        mutex_control::mutex_control() :
        _data()
        {
            
        }

        void mutex_control::lock_task(mutex const* m, IdType task_id) {
            auto& control_data = _data[m];

            control_data.task(task_id);
        }

        void mutex_control::unlock_task(mutex const* m) {
            auto& control_data = _data[m];

            control_data.task(0);       //TODO Invalid Task ID

            if (control_data.empty()) {
                _data.erase(m);
            }
        }

        void mutex_control::wait_for_mutex(mutex const* m, IdType task_id) {
            auto& control_data = _data[m];

            control_data.add_to_waitlist(task_id);
        }
    }
}