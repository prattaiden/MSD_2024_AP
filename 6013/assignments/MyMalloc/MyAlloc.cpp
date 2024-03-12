//
// Created by Aiden Pratt on 3/11/24.
//

#include "MyAlloc.h"
#include <sys/mman.h>
#include <iostream>


/*
 *The mmap system call is used to allocate memory.
 * The MAP_PRIVATE flag specifies that the mapping is private to the calling process,
 * and the MAP_ANONYMOUS flag indicates that the mapping is not backed by any file.
 *The PROT_READ | PROT_WRITE flags specify that the allocated memory should be readable and writable.
 */

void* MyAlloc::allocate(size_t bytesToAllocate) {
    void* allocatedMemory = mmap(nullptr, bytesToAllocate, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    mallocHash.insert(allocatedMemory, bytesToAllocate);
    return allocatedMemory;
}

void MyAlloc::deallocate(void* ptr) {
    size_t size = mallocHash.lazyDelete(ptr);


    if(size == 0){
        std::perror("lazy delete fail");
    }
    munmap(ptr, size);
}
