#include "../../include/detail/thread_pool.h"
#include <algorithm>

namespace gothreads {
    namespace detail {
        thread_pool::thread_pool() :
        _worker_threads(),
        _id(1),
        _max_threads(4)
        {
            
        }

        void thread_pool::schedule_task(task&& new_task) {
            _get_worker_thread();
        }

        worker_thread& thread_pool::_get_worker_thread() {
            if (_worker_threads.empty()) {
                return _allocate_worker_thread();
            }
            
            auto currentTasks = std::vector<size_t>();
            currentTasks.reserve(_worker_threads.size());

            for (auto const& pair : _worker_threads) {
                currentTasks.push_back(5);
            }

            if (_worker_threads.size() < _max_threads) {
                auto itToElem = std::find(currentTasks.begin(), currentTasks.end(), 0);

                //Check wether empty worker_thread is available
                if (itToElem != currentTasks.end()) {
                    auto index = itToElem - currentTasks.begin();
                    auto workerThreadIt = _worker_threads.begin();
                    std::advance(workerThreadIt, index);
                    return workerThreadIt->second;
                }

                //Allocate new worker_thread
                else {
                    return _allocate_worker_thread();
                }
            }
            else {
                auto itToElem = std::min_element(currentTasks.begin(), currentTasks.end());
                auto index = itToElem - currentTasks.begin();
                auto workerThreadIt = _worker_threads.begin();
                std::advance(workerThreadIt, index);
                return workerThreadIt->second;
            }
        }

        worker_thread& thread_pool::_allocate_worker_thread() {
            _worker_threads.insert_or_assign(_id++, worker_thread());
            return _worker_threads[_id];
        }
    }
}
