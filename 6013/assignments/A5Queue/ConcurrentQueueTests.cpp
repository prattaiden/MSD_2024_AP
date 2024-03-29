//
// Created by Aiden Pratt on 3/29/24.
//

#include "ConcurrentQueue.hpp"
#include <vector>
#include <iostream>
#include <thread>

bool testQueue( int num_producers, int num_consumers, int num_ints ) {
    std::vector<std::thread> threads;
    threads.reserve(num_producers + num_consumers);

    ConcurrentQueue<int> concurrentQueue;

    for(int i = 0; i < num_producers; i++){
        threads.push_back(std::thread([&concurrentQueue, num_ints]() {
            for (int j = 0; j < num_ints; j++) {
                concurrentQueue.enqueue(j);
            }
        }));
    }

    for(int o = 0 ; o < num_consumers ; o++ ){
        threads.push_back(std::thread([&concurrentQueue, num_ints](){
            for(int t = 0; t < num_ints; t++){
                int value;
                concurrentQueue.dequeue(&value);
            }
        }));
    }
    for(std::thread& thread : threads){
        thread.join();
    }
    return concurrentQueue.size() == (num_producers - num_consumers)*num_ints;
}


int main(int argc, char** argv){

    std::string argument = argv[1];
    int producers = std::stoi(argument);
    argument = argv[2];
    int consumers = std::stoi(argument);
    argument = argv[3];
    int num_ints = std::stoi(argument);

    if(testQueue(producers, consumers, num_ints)){
        std::cout << "tests passed\n";
    }
    else{
        std::cout << "failed test\n";
    }
}