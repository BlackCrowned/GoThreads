#pragma once
#include <unordered_map>
#include "task.h"

namespace gothreads {
    namespace detail {
        
        class task_pool {
            
            std::list<std::pair<size_t, task>> _list;
            size_t _id;
            mutable decltype(_list)::iterator _it;
        public:
            task_pool();

            size_t size() const;
            bool empty() const;

            task& next();
            task const& next() const;

            task& current() const;

            void erase_current_task();

            void add(task&& new_task);
        private:


        };

    }
}
