#include "dependencies/catch/single_include/catch.hpp"
#include "../include/gothreads.h"

TEST_CASE("Can construct 'go' class", "[contructable]") {
    gothreads::go([](int i) {return 0; }, 25);    //with return and argument
    gothreads::go([]() {return 0; });             //return only
    gothreads::go([]() {return; });               //basic void
}