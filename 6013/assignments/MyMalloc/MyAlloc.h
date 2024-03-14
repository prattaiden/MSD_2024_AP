//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Author: Aiden Pratt
//Date: 3/11/2034
//CS:6013
//My malloc header file with the declaration of the allocate and deallocate functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYMALLOC_MYALLOC_H
#define MYMALLOC_MYALLOC_H

#include "HashTable.h"


class MyAlloc {

public:
    MyAlloc();

    //hash table member variable
    HashTable mallocHash;

    void* allocate(size_t size);
    void deallocate(void* ptr);
};


#endif //MYMALLOC_MYALLOC_H
