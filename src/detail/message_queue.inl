namespace gothreads {
    namespace detail {
        
        template <class IdType>
        one_to_many_message_queue<IdType>::one_to_many_message_queue() :
        _container(),
        _mutex(),
        _cv(),
        _receiver_asleep(false)
        {
            
        }

        template <class IdType>
        void one_to_many_message_queue<IdType>::register_receiver(IdType receiver) {
            std::lock_guard<std::mutex> lk(_mutex);

            _container[receiver];
        }

        template <class IdType>
        void one_to_many_message_queue<IdType>::unregister_receiver(IdType receiver) {
            std::lock_guard<std::mutex> lk(_mutex);

            _check_receiver_exists();

            _container.erase(receiver);
        }

        template <class IdType>
        void one_to_many_message_queue<IdType>::broadcast(MessageType const& msg) {
            std::unique_lock<std::mutex> lk(_mutex);

            for (auto const& queue : _container) {
                queue.push(msg);
            }

            _conditional_notify(lk);
        }

        template <class IdType>
        void one_to_many_message_queue<IdType>::send(MessageType&& msg, IdType receiver) {
            std::unique_lock<std::mutex> lk(_mutex);

            _check_receiver_exists();

            _container[receiver].push(std::forward<MessageType>(msg));

            _conditional_notify(lk);
        }

        template <class IdType>
        typename one_to_many_message_queue<IdType>::MessageType one_to_many_message_queue<IdType>::receive(IdType receiver) {
            std::lock_guard<std::mutex> lk(_mutex);

            _check_receiver_exists();

            auto const& queue = _container[receiver];
            MessageType msg = std::move(queue.front());
            queue.pop();

            return msg;
        }

        template <class IdType>
        bool one_to_many_message_queue<IdType>::empty(IdType receiver) const {
            std::lock_guard<std::mutex> lk(_mutex);

            _check_receiver_exists();

            return _container[receiver].empty();
        }

        template <class IdType>
        void one_to_many_message_queue<IdType>::wait() const {
            std::unique_lock<std::mutex> lk(_mutex);

            _receiver_asleep = true;

            _cv.wait(lk);
            lk.unlock();
        }

        template <class IdType>
        void one_to_many_message_queue<IdType>::_conditional_notify(std::unique_lock<std::mutex>& lk) const {
            if (_receiver_asleep) {
                _receiver_asleep = false;
                lk.unlock();
                _cv.notify_all();
            }
            else {
                lk.unlock();
            }
        }

        template <class IdType>
        void one_to_many_message_queue<IdType>::_check_receiver_exists(IdType const& receiver) const {
            if(_container.count(receiver) > 0) {
                throw "one_to_many_message_queue: Receiver does not exist.";
            }
        }

    }
}