#include "../include/mutex.h"
#include "../include/gothreads.h"

namespace gothreads {

    namespace detail
    {
        mutex_data::mutex_data() :
            _locked(false),
            _owner(0),
            _queue()
        {

        }

        mutex_data::~mutex_data() {
            if (!empty()) {
                assert("Mutex still has tasks waiting. Tasks will be destroyed aswell");
            }
        }

        void mutex_data::locked(bool l) {
            _locked = l;
        }

        bool mutex_data::locked() const {
            return _locked;
        }

        void mutex_data::owner(size_t id) {
            _owner = id;
        }

        size_t mutex_data::owner() const {
            return _owner;
        }

        void mutex_data::push_task(detail::task&& t) {
            _queue.push(std::forward<detail::task>(t));
        }

        detail::task mutex_data::pop_task() {
            auto t = std::move(_queue.front());
            _queue.pop();
            return t;
        }

        size_t mutex_data::size() const {
            return _queue.size();
        }

        bool mutex_data::empty() const {
            return _queue.empty();
        }
    }

    mutex::mutex() : base(),
    _m(),
    _mutex_data()
    {
        
    }

    void mutex::lock() {
        std::unique_lock<std::mutex> lock(_m);

        while(_mutex_data.locked()) {
            _wait_for_mutex(lock, _mutex_data);
        }
        _mutex_data.owner(_thread_pool->current_task_id());
        _mutex_data.locked(true);

        lock.unlock();
    }

    bool mutex::try_lock() {
        std::lock_guard<std::mutex> lock(_m);

        if (!_mutex_data.locked()) {
            _mutex_data.owner(_thread_pool->current_task_id());
            _mutex_data.locked(true);
            return true;
        }
        return false;
    }

    void mutex::unlock() {
        std::lock_guard<std::mutex> lock(_m);

        _mutex_data.locked(false);
        _mutex_data.owner(0);
        if (!_mutex_data.empty()) {
            _thread_pool->broadcast_message(std::make_shared<detail::messages::unlock_task>(_mutex_data.pop_task()));
        }
    }

    void mutex::_wait_for_mutex(std::unique_lock<std::mutex>& lock, detail::mutex_data& data) const {
        _thread_pool->yield_task(std::this_thread::get_id(), std::make_shared<detail::messages::wait_for_mutex>(lock, data));
        lock.lock();
    }
}
