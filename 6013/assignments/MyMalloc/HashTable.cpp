//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Author: Aiden Pratt
//Date: 3/11/2034
//CS:6013
//cpp file of my hash table which is utilized in my malloc to store addresses and data of inputted sizes
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sys/mman.h>
#include "HashTable.h"

//HashTable Constructor
HashTable::HashTable() {
    this->capacity_ = intialCapacity_;
    this->size_ = 0;
    table_ = static_cast<TableEntry *>(mmap(nullptr, capacity_ * sizeof(TableEntry), PROT_READ | PROT_WRITE,
                                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
}

//HashTable Destructor
HashTable::~HashTable() {
    int tc = munmap(table_, capacity_);
    if(tc == TOMBSTONE){
        throw std::runtime_error("deconstruct failed");
    }
}

//hash method to place entry in the hash table
size_t HashTable::hash(void* entry) const {
    const char* key = reinterpret_cast<const char*>(entry);
    size_t hash = 0;

    while (*key) {
        hash += *key++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
//modulu with capacity to ensure hash is in range
    return hash % capacity_;
}

void* HashTable::find(void* entry) {
    size_t index = hash(entry);
    size_t ogIndex = index;

    //linear probing
    while(table_[index].address != nullptr && table_[index].address != entry) {
        //checking if address is not flagged as removed
        if (table_[index].address != reinterpret_cast<void *>(-1)) {
            //moving to next index
            index = (index + 1) % capacity_;
        }
        //check if looped around to whole table
        if(index == ogIndex){
            return nullptr;
        }
    }
    if (table_[index].address == entry && table_[index].address != reinterpret_cast<void*>(TOMBSTONE)) {
        return table_[index].address;
    }
    //if not found
    return nullptr;
}

void HashTable::grow() {
    //double capacity immediately
    //could also be done later in the method
    this->capacity_ *=2;

    // Create a new table twice the size
    TableEntry *newTable = static_cast<TableEntry *>(mmap(nullptr, capacity_ * sizeof(TableEntry), PROT_READ | PROT_WRITE,
                                                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));

    // Iterate through the current table and rehash into the new table
    for (size_t i = 0; i < size_; i++) {
        if (table_[i].address != nullptr && table_[i].address != reinterpret_cast<void*>(TOMBSTONE)) {
            size_t index = hash(table_[i].address);
            // Rehash the entry into the new table using its address and size
            while(newTable[index].address != nullptr && newTable[index].address != reinterpret_cast<void*>(TOMBSTONE)){
                index = (index + 1) % capacity_;
            }

            newTable[index].address = table_[i].address;
            newTable[index].size = table_[i].size;
        }
    }

    TableEntry* oldTable = table_;
    table_ = newTable;

    //deallocate the old table
    //capacity_ is currently new capacity, so /2 when deallocating
    int mun = munmap(oldTable, (capacity_/2) * sizeof(TableEntry));
    if(mun == -1){
        std::perror("munmap failed");
        exit(EXIT_FAILURE);
    }
}

void HashTable::insert(void* entry, size_t entrySize) {
    if(size_ == THRESHOLD_FACTOR * capacity_){
        //std::cout << "hit growth part\n";
        grow();
    }
    size_t index = hash(entry);
    while(table_[index].address != nullptr && table_[index].address != reinterpret_cast<void*>(TOMBSTONE)){
        index = (index + 1) % capacity_;
    }
    table_[index].address = entry;
    table_[index].size = entrySize;
    size_++;
}


//lazy delete from the hash table
//this method does not necessarily just clear the entire data
//it takes a pointer to the entry and replaces the address with a TOMBSTONE
//this TOMBSTONE = -1 and symbolizes a way the allocator can see if deleted data has been in this memory location
size_t HashTable::lazyDelete(void* entry) {
    size_t index = hash(entry);
    size_t originalIndex = index;
    size_t deletedSize = 0;  // Initialize deletedSize to 0

    while (table_[index].address != nullptr) {
        if (table_[index].address == entry) {
            // Mark the entry as deleted
            table_[index].address = reinterpret_cast<void*>(TOMBSTONE);
            size_--;
            deletedSize = table_[index].size;  // Get the size before deletion
            break;  // Break if entry found and marked as deleted
        }

        index = (index + 1) % capacity_;
        if (index == originalIndex) {
            break;  // Break if looped around without finding the entry
        }
    }
    return deletedSize;  // Return the size of the deleted block
}



