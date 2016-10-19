#pragma once
#include <memory>
#include <unordered_map>

namespace gothreads {
    namespace detail {
        class thread_pool {
            std::unordered_map<int, char> _worker_threads;

        public:

            thread_pool() = default;


        };
    }
}
