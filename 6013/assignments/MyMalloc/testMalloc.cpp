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
#include <ctime>

//todo write up notes
//grow size is something we could do


MyAlloc test;

//comment out this malloc and free to run the system version of malloc and free for the timing tests
//void* malloc(size_t size){
//    void* mem = test.allocate(size);
//    return mem;
//}
//void free(void* ptr){
//    test.deallocate(ptr);
//}


void testMalloc::TestMalloc1() {

    void *testPointerAddress;

    assert(test.mallocHash.capacity_ == 10);
    assert(test.mallocHash.size_ == 0);
    std::cout << "Test 1: \t set up test passed!\n";

    testPointerAddress = malloc(34);
    int nodeIndex = test.mallocHash.hash((testPointerAddress));

    // After allocating only one node into hash, the size should increase by one,
    // and capacity remains the same
    assert(test.mallocHash.capacity_ == 10);
    assert(test.mallocHash.size_ == 1);
    std::cout << "Test 1: \t allocation and size tests passed!\n";

    //the position of this node should not be a nullptr now
    assert(test.mallocHash.table_[nodeIndex].address != nullptr);
    std::cout << "Test 1: \t not nullptr test passed!\n";

    free(testPointerAddress);

    //after deallocation, this index should now be a tombstone, and size == 0
    assert(test.mallocHash.table_[nodeIndex].address == reinterpret_cast<void*>(TOMBSTONE));
    assert(test.mallocHash.size_ == 0);
    assert(test.mallocHash.capacity_ == 10);
    std::cout << "Test 1: \t deallocation and size tests passed!\n";
}

void testMalloc::TestMalloc2(){
    MyAlloc growthTest;
    void *growthAddresses[10];

// Now to test adding elements up to 7, which will be 70% capacity, forcing it to double
    for (int i = 0; i < 9; i++) {
        growthAddresses[i] = growthTest.allocate(10);
    }
// After allocating 8 elements, the size should be 8, and the capacity should be doubled
    assert(growthTest.mallocHash.size_ == 9);
    assert(growthTest.mallocHash.capacity_ == 20);
    std::cout << "Test 2: \t for loop for growing capacity passed! ";
}

//todo more test ideas:
//size of char, size of int, array of 1000, check if overlap?

//void testMalloc::TestMalloc3(){
//    MyAlloc arrayTestAlloc;
//    void* arrayOfAddresses[11];
//    // By inserting elements 11 times, it should reach growth threshold, and capacity should increase.
//    for (int i = 0; i < 11; i++) {
//        arrayOfAddresses[i] = arrayTestAlloc.allocate(10);
//    }
//    assert(arrayTestAlloc.mallocHash.capacity_ == 20);
//
//    //deallocate the things that have been allocated
//    for(int i = 0 ; i < 11; i++ ){
//        arrayTestAlloc.deallocate(arrayOfAddresses[i]);
//    }
//    assert(arrayTestAlloc.mallocHash.capacity_ == 20);
//    assert(arrayTestAlloc.mallocHash.size_ == 0);
//
//
//}

//void testMalloc::TestMalloc4(){
//    MyAlloc CharTest;
//    void* CharSize = malloc(1);
//    int index = CharTest.mallocHash.hash(CharSize);
//
//}



void testMalloc::HashTableInsertTests(){
    HashTable hashTable;
    assert(hashTable.capacity_ == 10);
//    assert(hashTable.size_ == 0);
    //setting to ten for testing purpose
    hashTable.capacity_ = 10;
    std::cout<< "initial capacity works\n";
    int val = 1;
    int val2 = 2;
    int val3 = 3;
    int val4 = 4;
    int val5 = 5;
    int val6 = 6;
    int val7 = 7;
    int val8 = 8;
    hashTable.insert(&val, val);
    assert(hashTable.size_ == 1);
    hashTable.insert(&val2, val2);
    assert(hashTable.size_ == 2);
    assert(hashTable.capacity_ == 10);
    hashTable.insert(&val3, val3);
    hashTable.insert(&val4, val4);
    hashTable.insert(&val5, val5);
    hashTable.insert(&val6, val6);
    hashTable.insert(&val7, val7);
    assert(hashTable.size_ == 7);
    assert(hashTable.capacity_ == 10);
    std::cout << "up to 7 added, next should grow.\n";
    hashTable.insert(&val8, val8);
    assert(hashTable.size_ == 8);
    assert(hashTable.capacity_ == 20);


    hashTable.lazyDelete(&val6);
    assert(hashTable.size_ == 7);
    std::cout << "lazy delete worked!";
}



void testMalloc::MallocTiming() {

    const int maxPower = 15;

    for (int power = 2; power <= maxPower; power++) {

        int n = 1000000;
        int size = pow(2, power);

        // Measure malloc time
        clock_t startMallocTime = clock();
        for(int i = 0; i < size; i++){
            malloc(size);
        }
        clock_t endMallocTime = clock();

        float time_used = endMallocTime - startMallocTime;
        float cpu_time = time_used / CLOCKS_PER_SEC;

        std::cout << size << "\t" <<  cpu_time * n << "\t" << "\n";

    }
}

void testMalloc::FreeTiming() {

    const int maxPower = 15;

    for (int power = 2; power <= maxPower; power++) {

        int n = 1000000;
        int size = pow(2, power);

        // Measure Free time
        clock_t startFreeTime = clock();
        for(int i = 0; i < size; i++){
            malloc(size);
        }
        clock_t endFreeTime = clock();

        float time_used = endFreeTime - startFreeTime;
        float cpu_time = time_used / CLOCKS_PER_SEC;

        std::cout <<  cpu_time * n << "\t" << "\n";

    }
}