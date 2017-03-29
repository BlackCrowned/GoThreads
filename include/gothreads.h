#pragma once
#include "detail/base.h"

namespace gothreads {
    class go : public detail::base {
    public:
        template <class FuncT, class ...Args>
        explicit go(FuncT function, Args&&... arguments);


    };

    class yield : public detail::base{
    public:
        yield();
    };

    class max_thread_count : public detail::base {
    public:
        max_thread_count(size_t n);
    };
}


#include "../src/gothreads.inl"
