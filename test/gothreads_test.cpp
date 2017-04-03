#include "dependencies/catch/single_include/catch.hpp"
#include "../include/gothreads.h"
#include "../include/mutex.h"
#include <iostream>
#include "../../../../../../../Program Files (x86)/Microsoft Visual Studio 14.0/VC/include/thread"

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
    //gothreads::max_thread_count(1);
    gothreads::mutex m;
    
    for (size_t i = 0; i < 10; i++) {
        gothreads::go([&]()
            {
                static size_t x = 0;
                //static gothreads::mutex m;
                m.lock();
                x++;
                std::cout << "Locked thread [" << x << "]" << std::endl;
                m.unlock();
                std::cout << "UnLocked thread" << std::endl;
                return;
            });
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}