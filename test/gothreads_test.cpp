#include "dependencies/Catch-1.5.7/catch.hpp"
#include "../include/gothreads.h"

#pragma comment(lib, "../Debug/GoThreads.lib")

TEST_CASE("Can construct 'go' class", "[contructable]") {
    auto t1 = gothreads::go([](int i) {return 0; }, 25);    //with return and argument
    auto t2 = gothreads::go([]() {return 0; });             //return only
    auto t3 = gothreads::go([]() {return; }, 25);           //basic void
}