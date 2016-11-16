#pragma once
#include <unordered_map>
#include "task.h"

namespace gothreads {
    namespace detail {
        
        class task_pool {
            
            std::unordered_map<size_t, task> _map;
            size_t _id;
        public:
            task_pool() = default;

            size_t size() const;
            bool empty() const;

            task& next();
            task const& next() const;

            void add(task&& new_task);
        private:


        };

    }
}
