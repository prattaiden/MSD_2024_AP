//
// Created by Aiden Pratt on 3/11/24.
//

#include "HashTable.h"

HashTable::HashTable(size_t initialCap) : capacity_(initialCap), size_(0) {
    table_ = new Data[capacity_];
}

HashTable::~HashTable() {

}

size_t HashTable::hash(void* entry) {
    return reinterpret_cast<size_t>(entry) % capacity_;
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

//todo deallocate the previous table?
HashTable HashTable::grow() {
    // Create a new table twice the size
    HashTable newTable;
    newTable.capacity_ *=2;

    //todo mmap?
    // Iterate through the current table and rehash into the new table
    for (size_t i = 0; i < size_ - 1; i++) {
        if (table_[i].address != nullptr && table_[i].address != reinterpret_cast<void*>(TOMBSTONE)) {
            // Rehash the entry into the new table using its address and size
            newTable.insert(table_[i].address, table_[i].size);
        }
    }

    return newTable;
}


void HashTable::insert(void* entry, size_t entrySize) {
    //entrySize check
    if(this->size_ >= THRESHOLD){
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

size_t HashTable::lazyDelete(void* entry) {
    size_t index = hash(entry);
    size_t originalIndex = index;

    while (table_[index].address != nullptr) {
        if (table_[index].address == entry) {
            size_t deletedSize = table_[index].size;  // Get the size before deletion
            table_[index].address = reinterpret_cast<void*>(TOMBSTONE);
            table_[index].size = 0;
            size_--;
            return deletedSize;  // Return the size of the deleted block
        }

        index = (index + 1) % capacity_;
        if (index == originalIndex) {
            break;  // Break if looped around without finding the entry
        }
    }
    return 0;  // Return 0 if the entry was not found
}


