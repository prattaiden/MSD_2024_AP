#pragma once

////////////////////////////////////////////////////////////////////////
//
// Author: Aiden Pratt
// Date: 3/25/2024
//
// CS 6013
//
//
//implementation of concurrent queue utilizing locks for the threads
////////////////////////////////////////////////////////////////////////

#include <cstddef>
#include <mutex>


template <typename T>
class ConcurrentQueue {

public:
    ConcurrentQueue() :
            head_( new Node{ T{}, nullptr } ), size_( 0 )
    {
        tail_ = head_;
    }

    int calculateTotalSize() const {
        int totalSize = 0;

        Node* current = head_->next;  // Skip the dummy head node

        while (current != nullptr) {
            totalSize++;
            current = current->next;
        }

        return totalSize;
    }

    //adds a new node at the tail of the linked list
    void enqueue( const T & x ) {
        Node *node = new Node(x, nullptr); //creating a new node from data passed in
//        if (size_ == 0) {
//            head_->next = node;
//        }
        {//lock scope
        std::unique_lock<std::mutex> lock(mutex_);
        tail_->next = node;
        tail_ = node;
        //size_++;
        }
    }

    //dequeue( T * ret ) removes a node from the head of the linked list,
    // and returns the data at the new head in the variable ret.
    // If the queue is empty, dequeue returns false.
    // If an element was dequeued successfully, dequeue returns true.
    bool dequeue( T * ret ) {
        {//lock scope
            std::unique_lock<std::mutex> lock(mutex_);
//            if (size_ == 0 || ret == nullptr) {
//                return false;
//            }
            *ret = head_->next->data; //data in the node to be removed
            Node *temp_head = head_;
            head_ = head_->next;//head is now pointing to the next next node
            delete temp_head; //remove that first node

            //size_--;
            return true;
        }
    }

    ~ConcurrentQueue() {

        while( head_ != nullptr ) {
            Node* temp = head_->next;
            delete head_;
            head_ = temp;
        }
    }



    int size() const { return size_; }

private:

    struct Node {
        //constructor for the Node
        explicit Node(const T& nodeData, Node* nextPtr = nullptr){
            this->data = nodeData;
            this->next = nextPtr;
        }

        T data;
        Node* next;
    };

public:
    Node* head_;
    Node* tail_;
    int size_;
    mutable std::mutex mutex_;
};
