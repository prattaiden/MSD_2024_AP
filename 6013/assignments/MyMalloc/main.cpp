//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Author: Aiden Pratt
//Date: 3/11/2034
//CS:6013
//main file for myMalloc implementation
//this program is my implementation of the memory allocator and free on the systems memory
//It contains a HashTable class which is utilized for growing and inserting data into the memory
//which is stored within the hash table based on a hash algorithm
//This program also contains a MyAlloc class which contain an implementation of allocate and deallocate
//these methods are utilized to overwrite the malloc and free methods from the system
//the testMalloc file is utilized to rigoursly test my HashTable and allocator to ensure it does not break ,
//and is allocated memory properly and storing it safely in the hashtable without overwriting other memory
//The testMalloc File also contains timing tests which were utilized to gather data in comparing the system malloc
//vs my malloc implementation, more info in conclusion.pdf!
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "testMalloc.h"
#include "testMalloc.h"
#include "MyAlloc.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    //uncomment for timing test!
   // testMalloc::MallocTiming();
    //testMalloc::FreeTiming();

    //other tests:
//    testMalloc::TestMalloc1();
//    testMalloc::TestMalloc2();
   //testMalloc::TestMalloc3();
   //this test below is for a large array size for memory
  // testMalloc::TestMalloc4();

  //tests below are for the hash table
    //testMalloc::HashTableCheck2();
//    testMalloc::HashTableInsertTests();


    return 0;
}
