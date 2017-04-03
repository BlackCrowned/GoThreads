#pragma once
#include <queue>
#include <memory>
#include <chrono>
#include <mutex>
#include "task.h"
#include <unordered_map>
#include <unordered_set>
#include <winerror.h>
#include <algorithm>
#include <algorithm>

namespace gothreads {
    class mutex;
    class mutex_data;
}

namespace gothreads {
    namespace detail {

        class message {
        public:
            message() = default;
            virtual ~message() = default;

            virtual type_info const& type() const = 0;
        };

        namespace messages {
            class exit_thread : public message{
            
                bool _force;
                std::chrono::milliseconds _time_to_exit;

            public:
                explicit exit_thread(bool force = false, std::chrono::milliseconds time_to_exit = std::chrono::milliseconds(0));
                virtual ~exit_thread() = default;

                type_info const& type() const override;

                bool force() const;
                std::chrono::milliseconds time_to_exit() const;
            };

            class add_task : public message{
                task _task;

            public:
                explicit  add_task(task&& t);
                virtual ~add_task() = default;

                type_info const& type() const override;

                task&& get();
            };

            class wait_for_mutex : public message {
            private:
                std::unique_lock<std::mutex>& _lock;
                mutex_data& _mutex_data;

            public:
                explicit wait_for_mutex(std::unique_lock<std::mutex>& lock, mutex_data& data);
                virtual ~wait_for_mutex() = default;

                type_info const& type() const override;

                void add_to_mutex_data(task&& t) const;
            };

            class unlock_task : public message {
                task _task;
                bool _empty;

            public:
                explicit  unlock_task(task&& t);
                virtual ~unlock_task() = default;

                type_info const& type() const override;

                task&& get();
                bool empty();
            };

        }
        
        template<class IdType>
        class message_queue {
            using QueueType = std::queue<std::shared_ptr<message>>;
            using ContainerType = std::vector<std::unique_ptr<QueueType>>;
            
            ContainerType _container;

            mutable std::mutex _mutex;
            std::condition_variable _cv;

            bool _receiver_asleep;

        public:
            message_queue();
            message_queue(message_queue<IdType> const& mq) = delete;
            message_queue(message_queue<IdType>&& mq) noexcept;

            message_queue<IdType>& operator=(message_queue<IdType>&& mq) noexcept;

            void send(IdType id, std::shared_ptr<message>&& msg);
            void broadcast(std::shared_ptr<message>&& msg);
            std::shared_ptr<message> receive(IdType id);

            IdType register_id();
            bool unregister_id(IdType id);

            bool empty(IdType id) const;

            void wait();

        private:

            ContainerType::iterator _find_queue(IdType id);
            ContainerType::const_iterator _cfind_queue(IdType id) const;
        };

        template<class IdType>
        class message_queue_wrapper {
            using MessageType = decltype(message_queue<IdType>().receive(IdType()));
            using MessageQueueType = std::shared_ptr<message_queue<IdType>>;

            MessageQueueType _mq;
            IdType _id;

        public:
            explicit message_queue_wrapper(MessageQueueType mq);
            message_queue_wrapper(message_queue_wrapper<IdType> const& mqw) = delete;
            message_queue_wrapper(message_queue_wrapper<IdType>&& mqw) noexcept = default;

            ~message_queue_wrapper() noexcept;

            message_queue_wrapper<IdType>& operator=(message_queue_wrapper<IdType>&& mq) noexcept;

            MessageQueueType message_queue() const;
            IdType id() const;
            
            void send(IdType id, MessageType&& msg);
            void broadcast(MessageType&& msg);
            MessageType receive();

            bool empty();

            void wait();
        };
    }
}

#include "../../src/detail/message_queue.inl"
