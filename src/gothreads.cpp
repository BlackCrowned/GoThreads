#include "../include/gothreads.h"

namespace gothreads {
    
    yield::yield() : base() {
        _thread_pool->yield_task(std::this_thread::get_id());
    }

    max_thread_count::max_thread_count(size_t n) : base(){
        _thread_pool->max_threads(n);
    }
}
