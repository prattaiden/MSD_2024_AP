#include <iostream>
#include "testMalloc.h"
#include "testMalloc.h"
#include "MyAlloc.h"

int main() {
    std::cout << "Hello, World!" << std::endl;


    testMalloc::Assertions();

    testMalloc::growthAssertions();

    return 0;
}
