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
            auto& q = *_find_queue(id);
            q->push(std::forward<std::shared_ptr<message>>(msg));
            if (_receiver_asleep) {
                _receiver_asleep = false;
                lk.unlock();                        //Unlock
                _cv.notify_all();                   //Notify
            }
            else {
                lk.unlock();                        //Unlock
            }
        }

        template <class IdType>
        void message_queue<IdType>::broadcast(std::shared_ptr<message>&& msg) {
            std::unique_lock<std::mutex> lk(_mutex); //Lock

            for (auto& q : _container) {
                q->push(msg);
            }

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
            auto& q = *_find_queue(id);
            auto msg = q->front();
            q->pop();

            return std::move(msg);
        }

        template <class IdType>
        IdType message_queue<IdType>::register_id() {
            auto q = std::make_unique<QueueType>();
            auto id = reinterpret_cast<IdType>(q.get());

            std::lock_guard<std::mutex> lk(_mutex);
            _container.emplace_back(std::move(q));

            return id;
        }

        template <class IdType>
        bool message_queue<IdType>::unregister_id(IdType id) {
            if (empty(id)) {
                std::lock_guard<std::mutex> lk(_mutex);
                _container.erase(_find_queue(id));
                return true;
            }
            return false;
        }


        template<class IdType>
        bool message_queue<IdType>::empty(IdType id) const {
            std::lock_guard<std::mutex> lk(_mutex);
            auto& q = *_cfind_queue(id);
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
            auto it = std::find_if(_container.begin(), _container.end(), [id](std::unique_ptr<QueueType> const& q)
            {
                return reinterpret_cast<IdType>(q.get()) == id;
            });

            if (it != _container.end()) {
                return it;
            }
            throw "No queue with requested id registered";

            return it;
        }

        template <class IdType>
        auto message_queue<IdType>::_cfind_queue(IdType id) const -> ContainerType::const_iterator{
            auto it = std::find_if(_container.cbegin(), _container.cend(), [id](std::unique_ptr<QueueType> const& q)
            {
                return reinterpret_cast<IdType>(q.get()) == id;
            });

            if (it != _container.cend()) {
                return it;
            }
            throw "No queue with requested id registered";

            return it;
        }

        template <class IdType>
        message_queue_wrapper<IdType>::message_queue_wrapper(MessageQueueType mq) :
        _mq(mq),
        _id(_mq->register_id())
        {
            
        }

        template <class IdType>
        message_queue_wrapper<IdType>::~message_queue_wrapper() noexcept {
            if (_mq.use_count()) {
                _mq->unregister_id(_id);
            }
        }

        template <class IdType>
        message_queue_wrapper<IdType>& message_queue_wrapper<IdType>::operator=(message_queue_wrapper<IdType>&& mqw) noexcept {
            _mq = std::move(mqw._mq);
            _id = std::move(mqw._id);
        }

        template <class IdType>
        typename message_queue_wrapper<IdType>::MessageQueueType message_queue_wrapper<IdType>::message_queue() const {
            return _mq;
        }


        template <class IdType>
        IdType message_queue_wrapper<IdType>::id() const {
            return _id;
        }


        template <class IdType>
        void message_queue_wrapper<IdType>::send(IdType id, MessageType&& msg) {
            _mq->send(id, std::forward<MessageType>(msg));
        }

        template <class IdType>
        void message_queue_wrapper<IdType>::broadcast(MessageType&& msg) {
            _mq->broadcast(std::forward<MessageType>(msg));
        }

        template <class IdType>
        typename message_queue_wrapper<IdType>::MessageType message_queue_wrapper<IdType>::receive() {
            auto msg = _mq->receive(_id);
            return msg;
        }

        template <class IdType>
        bool message_queue_wrapper<IdType>::empty() {
            return _mq->empty(_id);
        }

        template <class IdType>
        void message_queue_wrapper<IdType>::wait() {
            return _mq->wait();
        }
    }
}
