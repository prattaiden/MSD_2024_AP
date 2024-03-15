//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Author: Aiden Pratt
//Date: 3/11/2034
//CS:6013
//my alloc cpp file with the implementation of the allocate and the deallocate files
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MyAlloc.h"
#include <sys/mman.h>
#include <iostream>


/*
 *The mmap system call is used to allocate memory.
 * The MAP_PRIVATE flag specifies that the mapping is private to the calling process,
 * and the MAP_ANONYMOUS flag indicates that the mapping is not backed by any file.
 *The PROT_READ | PROT_WRITE flags specify that the allocated memory should be readable and writable.
 */

MyAlloc::MyAlloc() {
 mallocHash;
}
void* MyAlloc::allocate(size_t bytesToAllocate) {
    if(bytesToAllocate == 0)
    {
        std::perror("invalid entry");
        return nullptr;
    }
   // size_t numOfPage = (bytesToAllocate + 4096-1)/4096;
    void* allocatedMemory = mmap(nullptr, bytesToAllocate, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(allocatedMemory == MAP_FAILED){
        std::perror("failed mmap");
    }
    mallocHash.insert(allocatedMemory, bytesToAllocate);
    return allocatedMemory;
}

void MyAlloc::deallocate(void* ptr) {
    size_t size = mallocHash.lazyDelete(ptr);

    if(size != 0) {
        int rc = munmap(ptr, size);
        if (rc == -1)
        std::perror("failed at munmap");
    }

}


