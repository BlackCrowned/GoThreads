#include "../../include/detail/message_queue.h"

namespace gothreads {
    namespace detail {

        namespace messages
        {
            exit_thread::exit_thread(bool force, std::chrono::milliseconds time_to_exit) :
            _force(force),
            _time_to_exit(time_to_exit)
            {

            }

            type_info const& exit_thread::type() const {
                return typeid(exit_thread);
            }

            bool exit_thread::force() const {
                return _force;
            }

            std::chrono::milliseconds exit_thread::time_to_exit() const {
                return _time_to_exit;
            }

            add_task::add_task(task&& t) :
            _task(std::forward<task>(t))
            {
                
            }

            type_info const& add_task::type() const {
                return typeid(add_task);
            }

            task&& add_task::get() {
                return std::move(_task);
            }

            wait_for_mutex::wait_for_mutex(mutex const* m) :
            _m(m)
            {
                
            }

            type_info const& wait_for_mutex::type() const {
                return typeid(wait_for_mutex);
            }

            mutex const* wait_for_mutex::get() const {
                return _m;
            }

            unlock_task::unlock_task(task&& t) :
            _task(std::forward<task>(t)),
            _empty(false)
            {
                
            }

            type_info const& unlock_task::type() const {
                return typeid(unlock_task);
            }

            task&& unlock_task::get() {
                _empty = true;
                return std::move(_task);
            }

            bool unlock_task::empty() {
                return _empty;
            }
        }
    }
}