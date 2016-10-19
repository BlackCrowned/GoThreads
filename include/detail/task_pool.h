#pragma once
#include <unordered_map>
#include "task.h"

namespace gothreads {
    namespace detail {
        
        class task_pool {
            
            std::unordered_map<int, task> _map;
            
        public:
            task_pool() = default;

            bool empty() const;

            task& next();
            task const& next() const;
        private:


        };

    }
}
