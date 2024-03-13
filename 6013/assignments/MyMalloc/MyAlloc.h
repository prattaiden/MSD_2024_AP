//
// Created by Aiden Pratt on 3/11/24.
//

#ifndef MYMALLOC_MYALLOC_H
#define MYMALLOC_MYALLOC_H

#include "HashTable.h"


class MyAlloc {

public:
    MyAlloc();

    HashTable mallocHash;
    void* allocate(size_t size);
    void deallocate(void* ptr);
};


#endif //MYMALLOC_MYALLOC_H
