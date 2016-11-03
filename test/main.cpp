#define CATCH_CONFIG_RUNNER

#include "dependencies/Catch-1.5.7/catch.hpp"

int main(int argc, char* const argv[]) {
    int result = Catch::Session().run(argc, argv);

    getchar();
    return result;
}
