#include "../include/gothreads.h"

namespace gothreads {
    
    yield::yield() : base() {
        _thread_pool->yield_task(std::this_thread::get_id());
    }
}
