#pragma once

namespace gothreads {
    namespace detail {

        class task_pool;

        class scheduler {
            task_pool* _task_pool;
        public:
            explicit scheduler(task_pool* ptask_pool);

            void init();

            void run();

        private:
            
        };
    }
}
