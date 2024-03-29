//////////////////////////////////////////////////////////////////////////
////
//// Author: Aiden Pratt
//// Date: 3/25/2024
////
//// CS 6013
////
////
////
//////////////////////////////////////////////////////////////////////////
//
//#include <cassert>
//#include "iostream"
//#include "SerialQueue.hpp"
//
//void print_queue_tests(){
//    SerialQueue<int> queue_test;
//
//    //asserting that head and tail next are pointing to nothing on construction
//    std::cout << queue_test.head_->next << "\n";
//    std::cout << queue_test.tail_->next << "\n";
//
//    int val1 = 10;
//    queue_test.enqueue(val1);
//
//    //now that something as been added, head is pointing at that address
//    std::cout << queue_test.head_->next << "\n";
//    std::cout << queue_test.tail_ << "\n";
//    std::cout << queue_test.tail_->data;
//}
//
//void static_enqueue_test(){
//    SerialQueue<int> stat_queue;
//    stat_queue.enqueue(10);
//    assert(stat_queue.size() == 1);
//}
//
//void static_dequeue_test(){
//    SerialQueue<int> stat_queue2;
//    stat_queue2.enqueue(10);
//    assert(stat_queue2.size() == 1);
//    int value; //value at head to be removed
//    stat_queue2.dequeue(&value);
//    assert(value == 10);
//    assert(stat_queue2.size() == 0);
//}
//
//void static_test2(){
//    SerialQueue<int> stat_queue;
//    stat_queue.enqueue(10);
//    stat_queue.enqueue(20);
//    stat_queue.enqueue(30);
//    stat_queue.enqueue(5000);
//    stat_queue.enqueue(20);
//    assert(stat_queue.size() == 5);
//    int value;//this will act as the value at the head to be removed
//    stat_queue.dequeue(&value);
//    assert(value == 10);
//    stat_queue.dequeue(&value);
//    assert(value == 20);
//    assert(stat_queue.size() == 3);
//}
//
////todo DYNAMIC IS RUN IT THROUGH A LOOP WHERE YOU CAN CONTROL THE NUMBER OF INSERTS AND NUMBER OF DELETIONS
//
//void new_dynamic_enqueue_test(){
//    SerialQueue<int> *dyn_queue_char = new SerialQueue<int>();
//    for(int i = 0; i < 100; i++){
//        dyn_queue_char->enqueue(i);
//    }
//    assert(dyn_queue_char->size() == 100);
//}
//
//void new_dynamic_dequeue_test(){
//    SerialQueue<int> *dyn_queue_char = new SerialQueue<int>();
//    for(int i = 0; i < 100; i++){
//        dyn_queue_char->enqueue(i);
//    }
//    assert(dyn_queue_char->size() == 100);
//
//   int value;
//    for(int i = 0; i < 100; i++){
//        dyn_queue_char->dequeue(&value);
//        assert(value == i);
//    }
//}
//
//void dynamic_enqueue_test() {
//    SerialQueue<int>* dyn_queue = new SerialQueue<int>();
//    dyn_queue->enqueue(1);
//    assert(dyn_queue->size() == 1);
//}
//
//void dynamic_dequeue_test() {
//    SerialQueue<int>* dyn_queue2 = new SerialQueue<int>();
//    dyn_queue2->enqueue(10);
//    dyn_queue2->enqueue(20);
//    dyn_queue2->enqueue(30);
//
//    assert(dyn_queue2->size() == 3);
//
//    int value;
//    dyn_queue2->dequeue(&value);
//    assert(value == 10 && dyn_queue2->size() == 2);
//
//    dyn_queue2->dequeue(&value);
//    assert(value == 20 && dyn_queue2->size() == 1);
//
//    dyn_queue2->dequeue(&value);
//    assert(value == 30 && dyn_queue2->size() == 0);
//
//    delete dyn_queue2;
//
//}
//
//
//int main(){
//
//    //print_queue_tests();
//    new_dynamic_enqueue_test();
//    new_dynamic_dequeue_test();
//
//    static_test2();
//    static_enqueue_test();
//    static_dequeue_test();
//    dynamic_enqueue_test();
//    dynamic_dequeue_test();
//    std::cout << "tests passed!";
//    return 0;
//}
