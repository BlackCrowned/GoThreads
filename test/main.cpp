#define CATCH_CONFIG_RUNNER

struct IUnknown; //Fix compilation error with Clang/C2
#include "dependencies/catch/single_include/catch.hpp"

int main(int argc, char* const argv[]) {
    int result = Catch::Session().run(argc, argv);

    getchar();
    return result;
}
