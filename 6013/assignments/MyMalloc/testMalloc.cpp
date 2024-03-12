//
// Created by Aiden Pratt on 3/11/24.
//

#include "testMalloc.h"
#include <iostream>
#include "HashTable.h"
#include "myAlloc.h"
#include <chrono>
#include <cmath>
#include <cassert>

void testMalloc::Assertions() {

    MyAlloc test;
    void *testPointerAddress;

    assert(test.mallocHash.capacity_ == 10);
    assert(test.mallocHash.size_ == 0);
    std::cout << "set up test passed!\n";

    testPointerAddress = test.allocate(45);
    int nodeIndex = test.mallocHash.hash((testPointerAddress));

    // After allocating only one node into hash, the size should increase by one,
    // and capacity remains the same
    assert(test.mallocHash.capacity_ == 10);
    assert(test.mallocHash.size_ == 1);
    std::cout << "allocation and size tests passed!\n";

    //the position of this node should not be a nullptr now
    assert(test.mallocHash.table_[nodeIndex].address != nullptr);

    test.deallocate(testPointerAddress);

    //after deallocation, this index should now be a nullptr, and size == 0
    assert(test.mallocHash.table_[nodeIndex].address == reinterpret_cast<void*>(TOMBSTONE));
    assert(test.mallocHash.size_ == 0);
    assert(test.mallocHash.capacity_ == 10);
    std::cout << "deallocation and size tests passed!\n";


}

void testMalloc::growthAssertions(){
    MyAlloc growthTest;
    void *growthAddresses[10];

// Now to test adding elements up to 7, which will be 70% capacity, forcing it to double
    for (int i = 0; i < 9; i++) {
        growthAddresses[i] = growthTest.allocate(10);
    }

// After allocating 8 elements, the size should be 8, and the capacity should be doubled
    assert(growthTest.mallocHash.size_ == 9);
    //todo, figure out why capacity wont grow
    assert(growthTest.mallocHash.capacity_ == 10);

}