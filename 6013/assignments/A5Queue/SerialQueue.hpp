#pragma once

////////////////////////////////////////////////////////////////////////
//
// Author: Aiden Pratt
// Date: 3/25/2024
//
// CS 6013
//
//
//
////////////////////////////////////////////////////////////////////////

#include <cstddef>

template <typename T>
class SerialQueue {

public:
   SerialQueue() :
      head_( new Node{ T{}, nullptr } ), size_( 0 )
   {
      tail_ = head_;
   }

    //adds a new node at the tail of the linked list
   void enqueue( const T & x ) {
       Node* node = new Node(x, nullptr); //creating a new node from data passed in, pointing at nothing
       if(size_ == 0){
           head_->next = node;
       }
       tail_->next = node;
       tail_ = node;
       //node->next = head_->next;// todo ????? in 0?
       size_++;
   }

    //dequeue( T * ret ) removes a node from the head of the linked list,
    // and returns the data at the new head in the variable ret.
    // If the queue is empty, dequeue returns false.
    // If an element was dequeued successfully, dequeue returns true.
   bool dequeue( T * ret ) {
       if(size_ == 0 || ret == nullptr){
           return false;
       }
       *ret = head_->next->data; //data in the node to be removed
       Node* temp_head = head_->next;
        head_->next = temp_head->next;//head is now pointing to the next next node
       delete temp_head; //remove that first node

       //if removing last node, set tail to head
       if(head_->next == nullptr){
           tail_ = nullptr;//queue is empty now
       }
       size_--;
   }

   ~SerialQueue() {

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
};
