#pragma once
#include <condition_variable>
#include "task_data.h"
#include "message_queue.h"

namespace gothreads {
    namespace detail {

        class task_pool;

        class scheduler {
            task_pool* _task_pool;

            message_queue<cardinality::one_to_one>* _receiver_queue;
            message_queue<cardinality::one_to_one>* _sender_queue;

            task_data _task_data;
        public:
            explicit scheduler(task_pool* ptask_pool, message_queue<cardinality::one_to_one>* receiver_queue, message_queue<cardinality::one_to_one>* sender_queue);

            void init();

            void run();

        private:
            
        };
    }
}
