#include "../dependencies/Catch-1.5.7/catch.hpp"
#include "../../include/detail/thread_pool.h"


TEST_CASE("Can construct 'detail::thread_pool' class", "[constructable]") {
    auto t1 = gothreads::detail::thread_pool();
}

//TEST_CASE("", "") {
//    auto t1 = gothreads::detail::thread_pool();
//}