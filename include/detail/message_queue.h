#pragma once
#include <queue>
#include <memory>
#include <chrono>
#include <mutex>

namespace gothreads {
    namespace detail {

        class message {
            
        public:
            message() = default;
            virtual ~message() = default;

            virtual type_info type() const = 0;
        };

        namespace messages {
            class exit_thread : public message{
            
                bool _force;
                std::chrono::milliseconds _time_to_exit;

            public:
                explicit exit_thread(bool force = false, std::chrono::milliseconds time_to_exit = std::chrono::milliseconds(0));
                virtual ~exit_thread() = default;

                constexpr type_info type() const override;

                bool force() const;
                std::chrono::milliseconds time_to_exit() const;
            };

        }
        
        class message_queue {
            std::queue<std::unique_ptr<message>> _queue;

            mutable std::mutex _mutex;
            std::condition_variable _cv;

            bool _receiver_asleep;

        public:
            message_queue();

            void send(std::unique_ptr<message>&& msg);
            std::unique_ptr<message> receive();

            bool empty() const;

            void wait();
        };

    }
}
