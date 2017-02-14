namespace gothreads {
    namespace detail {
        
        template<class IdType>
        message_queue<IdType>::message_queue() :
        _container(),
        _mutex(),
        _cv(),
        _receiver_asleep(false)
        {

        }

        template<class IdType>
        message_queue<IdType>::message_queue(message_queue<IdType>&& mq) noexcept :
        _container(std::move(mq._container)),
        _mutex(),
        _cv(),
        _receiver_asleep(std::move(mq._receiver_asleep))
        {

        }

        template<class IdType>
        message_queue<IdType>& message_queue<IdType>::operator=(message_queue<IdType>&& mq) noexcept {
            _container = std::move(mq._container);
            _receiver_asleep = std::move(mq._receiver_asleep);

            return *this;
        }

        template<class IdType>
        void message_queue<IdType>::send(IdType id, std::shared_ptr<message>&& msg) {
            std::unique_lock<std::mutex> lk(_mutex); //Lock
            auto q = *_find_queue(id);
            q->push(msg);
            if (_receiver_asleep) {
                _receiver_asleep = false;
                lk.unlock();                        //Unlock
                _cv.notify_all();                   //Notify
            }
            else {
                lk.unlock();                        //Unlock
            }
        }

        template<class IdType>
        std::shared_ptr<message> message_queue<IdType>::receive(IdType id) {
            std::lock_guard<std::mutex> lk(_mutex);
            auto q = *_find_queue(id);
            auto msg = q->front();
            q->pop();

            return msg;
        }

        template <class IdType>
        IdType message_queue<IdType>::register_id() {
            auto q = std::make_unique<QueueType>();
            _container.emplace_back(q);

            return static_cast<IdType>(q.get());
        }

        template <class IdType>
        bool message_queue<IdType>::unregister_id(IdType id) {
            if (empty(id)) {
                _container.erase(_find_queue(id));
                return true;
            }
            return false;
        }


        template<class IdType>
        bool message_queue<IdType>::empty(IdType id) const {
            std::lock_guard<std::mutex> lk(_mutex);
            auto q = *_find_queue(id);
            return q->empty();
        }

        template<class IdType>
        void message_queue<IdType>::wait() {
            std::unique_lock<std::mutex> lk(_mutex);

            _receiver_asleep = true;
            _cv.wait(lk);

            lk.unlock();
        }

        template <class IdType>
        auto message_queue<IdType>::_find_queue(IdType id) -> ContainerType::iterator{
            auto it = std::find(_container.begin(), _container.end(), [id](std::unique_ptr<QueueType> const& q)
            {
                return static_cast<IdType>(q.get()) == id;
            });

            if (it != _container.end()) {
                return it;
            }
            throw "No queue with requested id registered";

            return it;
        }


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
