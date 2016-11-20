#include "../dependencies/catch/single_include/catch.hpp"
#include "../../include/detail/task.h"
TEST_CASE("Can construct 'task' class", "[constructable]") {
    auto t1 = gothreads::detail::task();    //Default constructor
    auto t2 = gothreads::detail::task([]() {return; });

    SECTION("Correct state is set") {
        REQUIRE(t1.state() == gothreads::detail::task_state::empty);
        REQUIRE(t2.state() == gothreads::detail::task_state::waiting);
    }
    SECTION("Correct executable state is set") {
        CHECK_FALSE(t1.executable());
        CHECK(t2.executable());
    }
}