#include "../../include/detail/thread_pool.h"
#include <algorithm>

namespace gothreads {
    namespace detail {
        thread_pool::thread_pool() :
        _worker_threads(),
        _thread_id_table(),
        _mutex_control(),
        _mq(new message_queue<size_t>()),
        _max_threads(4),
        _id(1)
        {
            
        }

        void thread_pool::schedule_task(task&& new_task) {
            auto& t = _get_worker_thread();
            
            t.schedule_task(std::forward<task>(new_task));
        }

        void thread_pool::yield_task(ThreadIdType const& id, task_state state) {
            _worker_threads.at(_thread_id_table[id]).yield_task(state);
        }

        size_t thread_pool::active_threads() const {
            return _worker_threads.size();
        }

        thread_pool::IdType thread_pool::current_task_id() const {
            return current_task_id(std::this_thread::get_id());
        }

        thread_pool::IdType thread_pool::current_task_id(ThreadIdType const& id) const {
            return _worker_threads.at(_thread_id_table.at(id)).get_task_pool().current().id();
        }

        worker_thread& thread_pool::_get_worker_thread() {
            if (_worker_threads.empty()) {
                return _allocate_worker_thread();
            }

            auto currentTasks = std::vector<size_t>();
            currentTasks.reserve(_worker_threads.size());

            for (auto const& pair : _worker_threads) {
                currentTasks.push_back(pair.second.current_tasks());
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
            auto& wt = _worker_threads.try_emplace(++_id, &_mutex_control, _mq).first->second;
            _thread_id_table[wt.id()] = _id;
            return wt;
        }
        
    }
}
