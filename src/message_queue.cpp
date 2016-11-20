#include "../include/detail/message_queue.h"

namespace gothreads {
    namespace detail {

        namespace messages
        {
            exit_thread::exit_thread(bool force, std::chrono::milliseconds time_to_exit) :
            _force(force),
            _time_to_exit(time_to_exit)
            {

            }

            type_info const& exit_thread::type() const {
                return typeid(exit_thread);
            }

            bool exit_thread::force() const {
                return _force;
            }

            std::chrono::milliseconds exit_thread::time_to_exit() const {
                return _time_to_exit;
            }
        }

        message_queue::message_queue() :
        _queue(),
        _mutex(),
        _cv(),
        _receiver_asleep(false)
        {

        }

        void message_queue::send(std::unique_ptr<message>&& msg) {
            std::unique_lock<std::mutex> lk(_mutex); //Lock
            _queue.push(std::forward<std::unique_ptr<message>>(msg));
            if (_receiver_asleep) {
                _receiver_asleep = false;
                lk.unlock();                        //Unlock
                _cv.notify_one();                   //Notify
            }
            else {
                lk.unlock();                        //Unlock
            }
        }

        std::unique_ptr<message> message_queue::receive() {
            std::lock_guard<std::mutex> lk(_mutex);
            std::unique_ptr<message> message = std::move(_queue.front());
            _queue.pop();

            return message;
        }

        bool message_queue::empty() const {
            std::lock_guard<std::mutex> lk(_mutex);
            return _queue.empty();
        }

        void message_queue::wait() {
            std::unique_lock<std::mutex> lk(_mutex);

            _receiver_asleep = true;
            _cv.wait(lk);

            lk.unlock();
        }
    }
}