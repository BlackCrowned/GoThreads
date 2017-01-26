#include "../../include/detail/base.h"

namespace gothreads {
    namespace detail {
        service_locator base::_service_locator = service_locator();

        base::base() :
            _thread_pool(_service_locator.get_thread_pool()),
            _mutex_control(_service_locator.get_mutex_control())
        {

        }
    }
}
