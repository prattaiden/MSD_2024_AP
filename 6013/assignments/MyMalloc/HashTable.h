//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Author: Aiden Pratt
//Date: 3/11/2034
//CS:6013
//header file for my hash table which is used to store memory addresses of certain sizes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MYMALLOC_HASHTABLE_H
#define MYMALLOC_HASHTABLE_H


#include <cstdlib>
#include <ctime>

//-1 is stored at the address when the memory there is freed
const int TOMBSTONE = -1;

//entry in the table
struct TableEntry{
    void* address;
    size_t size;

    TableEntry(){
        address = nullptr;
        size = 0;
    }
};
//-------------------------------hash table---------------------------------//
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
