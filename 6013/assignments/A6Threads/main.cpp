#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <cassert>
#include <iomanip>
#include <functional>
#include <functional>
#include <omp.h>

//todo clang++ -std=c++17 -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp main.cpp -o main


template<typename T>
struct result {
    T sum;
    long long time_taken;
};

int global_omp_sum;
//atomic variable which will hold the global sum
std::atomic<int> global_sum;


//------------------------------part 1-----------------------------------------//
//divide array into equal parts for each thread
//each thread calculates it's partial sum
template<typename T>
void partial_sum (T a[], size_t start, size_t end){
    T partial_sum = 0;
    for(size_t i = start; i < end; i++){
        partial_sum +=a[i];
    }
    global_sum.fetch_add(partial_sum, std::memory_order_relaxed);
};


template<typename T>
result<T> parallel_sum_std( T a[], size_t N, size_t num_threads ){
    //vector of threads
    std::vector<std::thread>threads;
    global_sum = 0;

    //start time for timing
    auto start_time = std::chrono::high_resolution_clock::now();

    //calculate chunk size of the array for each thread to math on
    size_t chunk_size = N / num_threads;
    size_t extra = N % num_threads;

    //start threads to calculate partial sums
    size_t start_index = 0;
    for(size_t i  = 0;i < num_threads; i++) {
        //sets the indeces for the thread in the thread vector to do it's summation on
        size_t endIndex = start_index + chunk_size + (i < extra ? 1 : 0);//adding in the extra
        //creating a new thread
        std::thread thread(partial_sum<T> , a, start_index, endIndex);
        threads.push_back(std::move(thread));
        //next thread will start at end of previous
        start_index = endIndex;
    }
    //join the threads when they are finished
    for(auto& thread :threads){
        thread.join();
    }
    //end timing on this summation process
    auto end_time = std::chrono::high_resolution_clock::now();

    //calculate time
    long long total = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    //return the result struct
    return {global_sum.load(), total};

}

//------------------------------part 2-----------------------------------------//
template<typename T>
result<T> parallel_sum_omp1( T a[], size_t N, size_t num_threads ) {

    auto startTime = std::chrono::high_resolution_clock::now();
    omp_set_num_threads(num_threads);

#pragma omp parallel
    {
        int local_sum = 0;
        //this is parallelizing the loop and performing a parallel reduction on the local_sum sum variable
#pragma omp for
        for (size_t i = 0; i < N; i++) {
            local_sum += a[i];
        }
        //only one thread can update the global sum at a time, atmoic
#pragma omp atomic
        global_omp_sum += local_sum;
    }

#pragma only one
    auto endTime = std::chrono::high_resolution_clock::now();
    long long total = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    return {global_omp_sum, total};
}

//------------------------------part 3-----------------------------------------//
template <typename T>
result<T> parallel_sum_omp_builtin(T a[], size_t N , size_t num_threads){
    auto start_time = std::chrono::high_resolution_clock::now();
    omp_set_num_threads(num_threads);
    int local_sum = 0;

    //reduction has openmp perform reduction with addition operatore and store result in local_sum
    #pragma omp parallel for reduction(+:local_sum)
    for(int i = 0; i < N ; i++){
        local_sum+=a[i];
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    return {local_sum, total};
}

//------------------------------ part 4-----------------------------------------//
//--testing each function--
//first check that the sum is correct

template<typename T>
void check_sum(std::function<result<T>(T[], size_t, size_t)>func) {
    size_t N = 1000;
    int a[N];
    int num_threads = 4;
    for (size_t i = 0; i < N; i++) {
        a[i] = 1;
    }
    //call the parallel sum function passed in to get the result
    auto result = func(a, N, num_threads);

    std::cout << "Computed sum: " << result.sum << std::endl;
    std::cout << "Expected sum: " << N << std::endl;
    assert(result.sum == N);
}

//-STRONG SCALING TESTING-//
template<typename T>
void strong_scaling(std::function<result<T>(T[], size_t, size_t)>func) {
    size_t N_strong = 2000000;
    int array_strong[N_strong];
    const size_t min_threads = 1;
    const size_t max_threads = 16;

    for (size_t num_threads = min_threads; num_threads <= max_threads; num_threads++) {
        auto start_time = std::chrono::high_resolution_clock::now();

        auto result = func(array_strong, N_strong, num_threads);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        std::cout << elapsed_time  << std::endl;
    }
}
//-WEAK SCALING TESTING-//
template<typename T>
void weak_scaling(std::function<result<T>(T[], size_t , size_t)> func){
    const size_t min_threads = 1;
    const size_t max_threads =16;
    const size_t min_N = 100000;
    const size_t max_N = 1000000;

    for (size_t num_threads = min_threads; num_threads <= max_threads; num_threads++) {
        for (size_t N = min_N; N <= max_N; N += min_N) {
            int a[N];
            for (size_t i = 0; i < N; i++) {
                a[i] = rand() % 100;
            }

            auto start_time = std::chrono::high_resolution_clock::now();
            auto result = func(a, N, num_threads);
            auto end_time = std::chrono::high_resolution_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

            std::cout << elapsed_time <<  std::endl;

        }
    }
}


//------------------------------ main-----------------------------------------//
int main() {
    std::cout << "Hello, World!" << std::endl;
    //-------------------------------CHECK SUMS-------------------------------------//
    check_sum<int>(parallel_sum_std<int>);
    check_sum<int>(parallel_sum_omp1<int>);
    check_sum<int>(parallel_sum_omp_builtin<int>);
    //---------------------------------SCALE TESTS------------------------------------//
    // todo I found that going up to 100,000 or 1,000,000 elements results in more dramatic and visual time saving with each of these methods

    //STRONG--
//    std::cout << "parallel_sum_std: \n";
//    strong_scaling<int>(parallel_sum_std<int>);
//    std::cout << "parallel_sum_omp1: \n";
//    strong_scaling<int>(parallel_sum_omp1<int>);
//    std::cout << "parallel_sum_omp_builtin: \n";
//    strong_scaling<int>(parallel_sum_omp_builtin<int>);

    //WEAK--
    weak_scaling<int>(parallel_sum_omp_builtin<int>);


//-------------------------------------------------------------------------------------------//

    //initial tests:
//    const size_t N = 1000000;
//    int* a = new int[N];
//    const size_t num_threads = 1;
//    for(size_t i = 0; i < N; i++){
//        a[i] = rand() % 100;
//    }
//    // Call parallel_sum_std function
//    //auto result = parallel_sum_std(a, N, num_threads);
//    //auto result2 = parallel_sum_omp1(a, N, num_threads);
//    auto result3 = parallel_sum_omp_builtin(a, N, num_threads);
//
//    // Output the sum and time taken
//    std::cout << "Sum: " << result3.sum << std::endl;
//    std::cout << "Time taken: " << result3.time_taken << " microseconds" << std::endl;
//
//    // Clean up
//    delete[] a;

    return 0;
}
