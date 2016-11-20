#pragma once
#include <condition_variable>
#include "task_data.h"
#include "message_queue.h"

namespace gothreads {
    namespace detail {

        class task_pool;

        class scheduler {
            task_pool* _task_pool;

            std::mutex* _cv_new_task_m;
            std::mutex* _task_pool_m;
            std::condition_variable* _cv_new_task;

            message_queue* _receiver_queue;
            message_queue* _sender_queue;

            task_data _task_data;
        public:
            explicit scheduler(task_pool* ptask_pool, std::mutex* cv_new_task_m, std::mutex* task_pool_m, std::condition_variable* cv_new_task, message_queue* receiver_queue, message_queue* sender_queue);

            void init();

            void run();

        private:
            
        };
    }
}
