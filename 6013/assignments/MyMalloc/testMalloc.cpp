//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Author: Aiden Pratt
//Date: 3/11/2034
//CS:6013
//test cpp file for testing the efficiency of my mallloc implementation vs the system's malloc implementation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "testMalloc.h"
#include <iostream>
#include "HashTable.h"
#include "myAlloc.h"
#include <chrono>
#include <cmath>
#include <cassert>
#include <ctime>



MyAlloc test;

//todo comment out this malloc and free to run the system version of malloc and free for the timing tests
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

void testMalloc::TestMalloc3(){
    MyAlloc arrayTestAlloc;
    void* arrayOfAddresses[11];
    // By inserting elements 11 times, it should reach growth threshold, and capacity should increase.
    for(int i = 0 ; i < 11; i++ ){
        arrayOfAddresses[i] = arrayTestAlloc.allocate(10);
    }
    assert(arrayTestAlloc.mallocHash.capacity_ == 20);
    assert(arrayTestAlloc.mallocHash.size_ == 11);
    std::cout << "capacity check worked!\n";

    //deallocate the things that have been allocated
    for(int i = 0 ; i < 11; i++ ){
        arrayTestAlloc.deallocate(arrayOfAddresses[i]);
        std::cout << "size: " << arrayTestAlloc.mallocHash.size_ << "\n";
    }
    assert(arrayTestAlloc.mallocHash.capacity_ == 20);
    assert(arrayTestAlloc.mallocHash.size_ == 0);
}

//this test is testing which much larger array sizes and more memory to allocate
void testMalloc::TestMalloc4(){
    MyAlloc arrayTestAlloc;
    void* arrayOfAddresses[10000];
    // By inserting elements 11 times, it should reach growth threshold, and capacity should increase.
    for(int i = 0 ; i < 9999; i++ ){
        arrayOfAddresses[i] = arrayTestAlloc.allocate(10000);
    }

    assert(arrayTestAlloc.mallocHash.capacity_ == 20480);
    assert(arrayTestAlloc.mallocHash.size_ == 9999);
    std::cout << "capacity check worked!\n";

    //deallocate the things that have been allocated
    for(int i = 0 ; i < 9999; i++ ){
        arrayTestAlloc.deallocate(arrayOfAddresses[i]);
       // std::cout << "size: " << arrayTestAlloc.mallocHash.size_ << "\n";
    }
    assert(arrayTestAlloc.mallocHash.capacity_ == 20480);
    assert(arrayTestAlloc.mallocHash.size_ == 0);
}

//testing the hashtable works
void testMalloc::HashTableCheck2(){
    HashTable hashTable;
    //checking that printing out an empty hash table is all null ptr, address is 0
    for(int i = 0; i < hashTable.capacity_; i++){
        std::cout << hashTable.table_[i].address << "\n";
    }
    int val = 543;
    hashTable.insert(&val, val);

    //print again after adding one value
    for(int i = 0; i < hashTable.capacity_; i++){
        std::cout << "added one value: " << hashTable.table_[i].address << "\n";
    }

    int val2 = 444;
    int val3 = 342;
    int val4 = 888;
    hashTable.insert(&val2, val2);    hashTable.insert(&val3, val3);    hashTable.insert(&val4, val4);

    //added more values
    for(int i = 0; i < hashTable.capacity_; i++){
        std::cout << "added more: " << hashTable.table_[i].address << "\n";
    }


}


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


//---------------------------------TIMING TESTS------------------------------//

//uncomment these tests in main
//their respective size and times in microseconds will be printed to the console
//at the top of this file, uncomment out the FREE and MALLOC overwritten methods in order to see -
//MyAlloc's implementation

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