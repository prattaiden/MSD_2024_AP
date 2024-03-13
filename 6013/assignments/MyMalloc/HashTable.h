//
// Created by Aiden Pratt on 3/11/24.
//

#ifndef MYMALLOC_HASHTABLE_H
#define MYMALLOC_HASHTABLE_H


#include <cstdlib>
#include <ctime>

const int TOMBSTONE = -1;


struct TableEntry{
    void* address;
    size_t size;

    TableEntry(){
        address = nullptr;
        size = 0;
    }
};

class HashTable {

private:
    size_t intialCapacity_ = 10;

    void* find(void* entry);

    const double THRESHOLD_FACTOR = 0.7;

public:

    explicit HashTable();

    ~HashTable();

    void insert(void* i, size_t entrySize);

    size_t lazyDelete(void* entry);
    size_t hash(void *entry) const;

    size_t capacity_;
    size_t size_;
    TableEntry* table_;

    void grow();
};



#endif //MYMALLOC_HASHTABLE_H
