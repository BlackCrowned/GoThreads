#include "../../include/detail/task_pool.h"

namespace gothreads {
    namespace detail {
        size_t task_pool::size() const {
            return _map.size();
        }

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

        void task_pool::add(task&& new_task) {
            _map.emplace(++_id, std::forward<task>(new_task));
        }
    }
}
