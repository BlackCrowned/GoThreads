#include "../../include/detail/worker_thread.h"
#include "../../include/detail/thread_pool.h"

namespace gothreads {
    namespace detail {

        worker_thread::worker_thread() :
        _thread(),
        _task_pool(),
        _mq(std::make_shared<message_queue<size_t>>()),
        _scheduler(&_task_pool, _mq.message_queue()),
        _scheduler_mq_id(_scheduler.message_queue_id())
        {
            generic::delegate<void()> del(this, &worker_thread::_thread_entry);
            _thread = std::move(std::thread(del));
        }

        worker_thread::~worker_thread() {
            _mq.send(_scheduler_mq_id, std::make_unique<messages::exit_thread>(true));
            _thread.join();
        }

        void worker_thread::schedule_task(task&& new_task) {
            _mq.send(_scheduler_mq_id, std::make_unique<messages::add_task>(std::forward<task>(new_task)));
        }

        void worker_thread::yield_task(task_state state) const {
            _task_pool.current().yield(state);
        }

        void worker_thread::wait_for_mutex(mutex const* mutex) {
            _task_pool.current().yield(task_state::blocking);
        }

        void worker_thread::reschedule_task() {
            _task_pool.current().yield(task_state::reschedule);
        }

        task_pool const& worker_thread::get_task_pool() const {
            return _task_pool;
        }

        task_pool& worker_thread::get_task_pool() {
            return _task_pool;
        }

        size_t worker_thread::current_tasks() const {
            return _task_pool.size();
        }

        worker_thread::IdType worker_thread::id() const {
            return _thread.get_id();
        }

        void worker_thread::_thread_entry() {
            _scheduler.init();

            _scheduler.run();

        }
    }
}
