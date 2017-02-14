#include "../../include/detail/task_pool.h"

namespace gothreads {
    namespace detail {
        task_pool::task_pool() :
            _list(),
            _id(0),
            _it(_list.begin())
        {
            
        }

        size_t task_pool::size() const {
            return _list.size();
        }

        bool task_pool::empty() const {
            return _list.empty();
        }

        task& task_pool::next(){
            if (_it != _list.end()) {
                ++_it;
            }
            if (_it == _list.end()) {
                _it = _list.begin();
            }
            return _it->second;
        }

        task const& task_pool::next() const {
            class notimplemented {};
            throw notimplemented();
            return task();
        }

        task& task_pool::current() {
            return _it->second;
        }

        task& task_pool::current() const {
            return _it->second;
        }

        void task_pool::erase_current_task() {
            if (_it != _list.end() && _it != _list.begin()) {
                _list.erase(_it--);
            }
            else if (_it != _list.end()){
                _list.erase(_it);
                _it = _list.begin();
            }
            else {
                assert(false);
            }
        }

        task task_pool::release_current_task() {
            auto t = std::move(current());
            
            erase_current_task();

            return t;
        }

        void task_pool::add(task&& new_task) {
            _list.emplace_back(std::make_pair(++_id, std::forward<task>(new_task)));
        }
    }
}
