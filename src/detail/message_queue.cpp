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
        }
    }
}