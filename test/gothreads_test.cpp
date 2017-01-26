#include "dependencies/catch/single_include/catch.hpp"
#include "../include/gothreads.h"
#include "../include/mutex.h"
#include <iostream>

TEST_CASE("Can construct 'go' class", "[contructable]") {
    gothreads::go([=](int i) {return 0; }, 25);    //with return and argument
    gothreads::go([&]() {return 0; });             //return only
    gothreads::go([]() {return; });                //basic void
}

TEST_CASE("Can yield inside of task", "[feature]") {
    for (size_t i = 0; i < 10; i++)
    {
        gothreads::go([]()
        {
            gothreads::yield();
        });
    }
}

TEST_CASE("Can use mutexes", "[feature]") {
    for (size_t i = 0; i < 10; i++) {
        gothreads::mutex m;

        gothreads::go([&]()
        {
            static size_t x = 0;
            m.lock();
            x++;
            std::cout << "Locked thread [" << x << "]" << std::endl;
            //m.unlock();
            std::cout << "UnLocked thread" << std::endl;
        });
    }
}