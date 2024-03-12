//
// Created by Aiden Pratt on 3/11/24.
//

#ifndef MYMALLOC_MYALLOC_H
#define MYMALLOC_MYALLOC_H

#include "HashTable.h"


class MyAlloc {

public:
    void* allocate(size_t size);

    HashTable mallocHash;

    void deallocate(void* ptr);
};


#endif //MYMALLOC_MYALLOC_H
