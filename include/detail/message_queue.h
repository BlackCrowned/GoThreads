#pragma once
#include <queue>
#include <memory>
#include <chrono>
#include <mutex>
#include "task.h"
#include <unordered_map>
#include <unordered_set>
#include <winerror.h>

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

        }
        
        class message_queue {
            std::queue<std::unique_ptr<message>> _queue;

            mutable std::mutex _mutex;
            std::condition_variable _cv;

            bool _receiver_asleep;

        public:
            message_queue();
            message_queue(message_queue const& mq) = delete;
            message_queue(message_queue&& mq) noexcept;

            message_queue& operator=(message_queue&& mq) noexcept;

            void send(std::unique_ptr<message>&& msg);
            std::unique_ptr<message> receive();

            bool empty() const;

            void wait();
        };

        template<class IdType>
        class one_to_many_message_queue {
            using MessageType = std::unique_ptr<message>;
            
            std::unordered_map<IdType, std::queue<MessageType>> _container;

            mutable std::mutex _mutex;
            mutable std::condition_variable _cv;

            mutable bool _receiver_asleep;

        public:
            one_to_many_message_queue();
            one_to_many_message_queue(one_to_many_message_queue const& mq) = delete;
            one_to_many_message_queue(one_to_many_message_queue&& mq) = delete;

            void register_receiver(IdType receiver);
            void unregister_receiver(IdType receiver);

            void broadcast(MessageType const& msg);

            void send(MessageType&& msg, IdType receiver);

            MessageType receive(IdType receiver);

            bool empty(IdType receiver) const;

            void wait() const;

        private:
            void _conditional_notify(std::unique_lock<std::mutex>& lk) const;
            void _check_receiver_exists(IdType const& receiver) const;
        };
    }
}

#include "../../src/detail/message_queue.inl"