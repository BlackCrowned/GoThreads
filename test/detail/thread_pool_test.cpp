#include "../dependencies/Catch-1.5.7/catch.hpp"
#include "../../include/detail/thread_pool.h"
#include <iostream>


TEST_CASE("Can construct 'detail::thread_pool' class", "[constructable]") {
    auto t1 = gothreads::detail::thread_pool();

    SECTION("No worker_threads active") {
        REQUIRE(t1.active_threads() == 0);
    }
}

TEST_CASE("Method '.schedule_task' is correctly implemented", "[method]") {
    auto t1 = gothreads::detail::thread_pool();

    for (size_t i = 0; i < 10; i++)
    {
        t1.schedule_task(gothreads::detail::task([](size_t n)
        {
            static size_t x = 0;
            std::cout << x++ << ": queued up as " << n << ". item" << std::endl;
        }, i));
    }

    REQUIRE(t1.active_threads() == 4);
}