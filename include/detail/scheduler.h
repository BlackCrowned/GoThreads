#pragma once
#include <condition_variable>

namespace gothreads {
    namespace detail {

        class task_pool;

        class scheduler {
            task_pool* _task_pool;

            std::mutex* _cv_new_task_m;
            std::mutex* _task_pool_m;
            std::condition_variable* _cv_new_task;

            int* _state;
        public:
            explicit scheduler(task_pool* ptask_pool, std::mutex* cv_new_task_m, std::mutex* task_pool_m, std::condition_variable* cv_new_task, int* state);

            void init();

            void run();

        private:
            
        };
    }
}
