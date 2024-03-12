//
// Created by Aiden Pratt on 3/11/24.
//

#ifndef MYMALLOC_HASHTABLE_H
#define MYMALLOC_HASHTABLE_H


#include <cstdlib>
#include <ctime>

const int TOMBSTONE = -1;


struct Data{
    void* address;
    size_t size;
};

class HashTable {

private:
    size_t intialCapacity_;

    void* find(void* entry);

    const double THRESHOLD = capacity_ * .7;

public:

    HashTable(size_t initalCap = 10);

    ~HashTable();

    void insert(void* i, size_t entrySize);

    size_t lazyDelete(void* entry);
    size_t hash(void *entry);

    size_t capacity_;
    size_t size_;
    Data* table_;

    HashTable grow();
};



#endif //MYMALLOC_HASHTABLE_H
