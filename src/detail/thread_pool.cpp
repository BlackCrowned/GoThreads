#include "../../include/detail/thread_pool.h"
#include "../../include/detail/task_pool.h"

namespace gothreads {
    namespace detail {
        bool task_pool::empty() const {
            return _map.empty();
        }

        task& task_pool::next() {
            class notimplemented {};
            throw notimplemented();
            return task();
        }

        task const& task_pool::next() const {
            class notimplemented {};
            throw notimplemented();
            return task();
        }
    }
}
