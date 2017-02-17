#pragma once
#include <condition_variable>
#include "task_data.h"
#include "message_queue.h"
#include "mutex_control.h"

namespace gothreads {
    namespace detail {

        class task_pool;

        class scheduler {
            task_pool* _task_pool;
            mutex_control* _mutex_control;
            
            message_queue_wrapper<size_t> _mq;

            task_data _task_data;
        public:
            explicit scheduler(task_pool* ptask_pool, mutex_control* pmutex_control, std::shared_ptr<message_queue<size_t>> mq_ptr);

            void init();

            void run();

            size_t message_queue_id() const;

        private:
            
        };
    }
}
