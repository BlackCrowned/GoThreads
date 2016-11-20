#include "../dependencies/catch/single_include/catch.hpp"
#include "../../include/detail/stack.h"

TEST_CASE("Can construct 'stack' class", "[constructable]") {
    auto s1 = gothreads::detail::stack::create(0);
    auto s2 = gothreads::detail::stack::create(100);
    auto s3 = gothreads::detail::stack::create<std::allocator<int>>(100);
    
    SECTION("Smart Pointer holds reference") {
        REQUIRE(s1);
        REQUIRE(s2);
        REQUIRE(s3);
    }
    
    SECTION("Correct capacity is set") {
        REQUIRE(s1->capacity() == 0);
        REQUIRE(s2->capacity() == 100);
        REQUIRE(s3->capacity() == 100);
    }
    
    SECTION("Size equals capacity") {
        REQUIRE(s1->size() == s1->capacity());
        REQUIRE(s2->size() == s2->capacity());
        REQUIRE(s3->size() == s3->capacity());
    }

    SECTION("Holds no data pointer") {
        REQUIRE(s1->data() == nullptr);
    }

    SECTION("Holds data pointer") {
        REQUIRE(s2->data());
        REQUIRE(s3->data());
    }
}

TEST_CASE("Method '.reserve' is correctly implemented", "[method]") {
    auto s1 = gothreads::detail::stack::create(0);
    auto s2 = gothreads::detail::stack::create(100);
    auto s3 = gothreads::detail::stack::create<std::allocator<int>>(100);
    
    SECTION("Empty stack is initialized") {
        s1->reserve(50);
        REQUIRE(s1->capacity() == 50);
        REQUIRE(s1->size() == 0);
    }
    SECTION("Reserve increases capacity but not size") {
        s2->reserve(150);
        REQUIRE(s2->capacity() == 150);
        REQUIRE(s2->size() == 100);
    }
    SECTION("Reserve does not reduce capacity") {
        s3->reserve(50);
        REQUIRE(s3->capacity() == 100);
        REQUIRE(s3->size() == 100);
    }
}

TEST_CASE("Method '.resize' is correctly implemented", "[method]") {
    auto s1 = gothreads::detail::stack::create(0);
    auto s2 = gothreads::detail::stack::create(100);
    auto s3 = gothreads::detail::stack::create<std::allocator<int>>(100);

    SECTION("Empty stack is initialized") {
        s1->resize(50);
        REQUIRE(s1->capacity() == 50);
        REQUIRE(s1->size() == 50);
    }

    SECTION("Resize increases both capacity and size") {
        s2->resize(150);
        REQUIRE(s2->capacity() == 150);
        REQUIRE(s2->size() == 150);
    }
    SECTION("Resize does not reduce capacity") {
        s3->resize(50);
        REQUIRE(s3->capacity() == 100);
        REQUIRE(s3->size() == 50);
    }
}

TEST_CASE("Method '.shrink_to_fit()' is correctly implemented", "[method]") {
    auto s1 = gothreads::detail::stack::create(100);

    SECTION("Shrink_to_fit reduces capacity") {
        s1->resize(50);
        s1->shrink_to_fit();
        REQUIRE(s1->size() == 50);
        REQUIRE(s1->capacity() == s1->size());
    }
}