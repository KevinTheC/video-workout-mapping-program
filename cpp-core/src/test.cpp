#include "controller.h"
#include <cassert>
#include <iostream>

int main() {
    assert(testIncrement(0) == 2);
    assert(testIncrement(5) == 7);
    std::cout << "[cpp-core] All tests passed!" << std::endl;
    // assert(testIncrement(3) == 1 && "Failed on purpose");
    return 0;
}