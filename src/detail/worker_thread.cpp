#include "../../include/detail/worker_thread.h"
#include "../../include/detail/thread_pool.h"

namespace gothreads {
    namespace detail {

        worker_thread::worker_thread() :
            _thread(),
            _task_pool(),
            _scheduler(&_task_pool, &_sender_queue, &_receiver_queue),
            _sender_queue(),
            _receiver_queue()
        {
            generic::delegate<void()> del(this, &worker_thread::_thread_entry);
            _thread = std::move(std::thread(del));
        }

        worker_thread::~worker_thread() {
            _sender_queue.send(std::make_unique<messages::exit_thread>(true));
            _thread.join();
        }

        void worker_thread::schedule_task(task&& new_task) {
            _sender_queue.send(std::make_unique<messages::add_task>(std::forward<task>(new_task)));
        }

        void worker_thread::yield_task() {
            _task_pool.current().yield();
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
