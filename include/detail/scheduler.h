#pragma once
#include <condition_variable>
#include "task_data.h"

namespace gothreads {
    namespace detail {

        class task_pool;
        class message_queue;

        class scheduler {
            task_pool* _task_pool;

            message_queue* _receiver_queue;
            message_queue* _sender_queue;

            task_data _task_data;
        public:
            explicit scheduler(task_pool* ptask_pool, message_queue* receiver_queue, message_queue* sender_queue);

            void init();

            void run();

        private:
            
        };
    }
}
