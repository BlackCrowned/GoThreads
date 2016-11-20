#define CATCH_CONFIG_RUNNER

#ifdef Debug
#pragma comment(lib, "../Debug/GoThreads.lib")
#else
#pragma comment(lib, "../Release/GoThreads.lib")
#endif

struct IUnknown; //Fix compilation error with Clang/C2
#include "dependencies/catch/single_include/catch.hpp"

int main(int argc, char* const argv[]) {
    int result = Catch::Session().run(argc, argv);

    getchar();
    return result;
}
