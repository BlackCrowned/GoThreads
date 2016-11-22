#include "../include/gothreads.h"

namespace gothreads {
    
    detail::service_locator detail::base::_service_locator = detail::service_locator();

    detail::base::base() :
    _thread_pool(_service_locator.get_thread_pool())
    {
        
    }

    yield::yield() : base() {
        _thread_pool->yield_task(std::this_thread::get_id());
    }
}
