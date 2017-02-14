#include "../../include/detail/service_locator.h"


namespace gothreads {
    namespace detail {

        std::shared_ptr<thread_pool> service_locator::get_thread_pool() const {
            if (_thread_pool.use_count() == 0) {
                _thread_pool = std::make_shared<thread_pool>();
            }
            return _thread_pool;
        }

        std::shared_ptr<mutex_control> service_locator::get_mutex_control() const {
            if (_mutex_control.use_count() == 0) {
                _mutex_control = std::make_shared<mutex_control>();
            }
            return  _mutex_control;
        }
    }
}
