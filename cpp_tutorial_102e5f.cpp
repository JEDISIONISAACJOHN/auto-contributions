//------------------------------------------------------------------------------
// Learning Objective:
// This tutorial introduces the fundamental concepts of asynchronous programming
// in C++ using `std::async`. Asynchronous programming allows us to run tasks
// in the background without blocking the main thread, making our applications
// more responsive and efficient. We will learn how to launch a task
// asynchronously and retrieve its result.
//------------------------------------------------------------------------------

#include <iostream>  // For standard input/output operations like std::cout
#include <future>    // For std::async, std::future, and std::launch policy
#include <chrono>    // For std::chrono::seconds, used for simulating work
#include <thread>    // For std::this_thread::sleep_for, to pause execution

// A simple function that simulates a time-consuming task.
// It takes an integer, sleeps for a bit, and then returns the integer squared.
int calculate_square_async(int number) {
    std::cout << "Worker thread: Starting calculation for " << number << "..." << std::endl;

    // Simulate some work being done. In a real application, this could be
    // network requests, file I/O, heavy computations, etc.
    std::this_thread::sleep_for(std::chrono::seconds(2));

    int result = number * number;
    std::cout << "Worker thread: Calculation for " << number << " finished. Result: " << result << std::endl;
    return result; // The result of our asynchronous operation
}

int main() {
    std::cout << "Main thread: Starting the program." << std::endl;

    // --- Introducing std::async ---
    // std::async is a function template that launches a function asynchronously.
    // It returns a std::future object, which will eventually hold the result
    // of the asynchronous function call.

    // We launch our function 'calculate_square_async' with the argument 5.
    // std::launch::async tells std::async to run the function on a new thread.
    // This is the default behavior if no policy is specified, but it's good
    // practice to be explicit.
    std::future<int> future_result = std::async(std::launch::async, calculate_square_async, 5);

    // While the asynchronous task is running in the background, the main thread
    // can continue executing other operations.
    std::cout << "Main thread: Doing other work while the calculation is in progress..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate more main thread work
    std::cout << "Main thread: Finished doing other work." << std::endl;

    // --- Retrieving the result from std::future ---
    // The .get() method on a std::future object blocks the current thread
    // until the asynchronous task completes and returns its result.
    // If the task has already finished, .get() returns immediately.
    // IMPORTANT: You can only call .get() once on a std::future.

    std::cout << "Main thread: Waiting for the asynchronous calculation to finish and getting the result." << std::endl;
    try {
        int result = future_result.get(); // This will block until calculate_square_async finishes.
        std::cout << "Main thread: Asynchronous calculation result: " << result << std::endl;
    } catch (const std::exception& e) {
        // In a real scenario, you might want to handle exceptions thrown by the async task.
        std::cerr << "Main thread: An exception occurred during asynchronous execution: " << e.what() << std::endl;
    }

    std::cout << "Main thread: Program finished." << std::endl;
    return 0;
}

// --- How to Compile and Run ---
// 1. Save the code as a .cpp file (e.g., async_tutorial.cpp).
// 2. Compile using a C++11 compliant compiler (or newer), like g++:
//    g++ async_tutorial.cpp -o async_tutorial -std=c++11 -pthread
//    (The -pthread flag is important for linking the threading library)
// 3. Run the executable:
//    ./async_tutorial
//
// --- Expected Output (order of "Doing other work" messages might vary slightly) ---
// Main thread: Starting the program.
// Main thread: Doing other work while the calculation is in progress...
// Worker thread: Starting calculation for 5...
// Main thread: Finished doing other work.
// Main thread: Waiting for the asynchronous calculation to finish and getting the result.
// Worker thread: Calculation for 5 finished. Result: 25
// Main thread: Asynchronous calculation result: 25
// Main thread: Program finished.
//
// --- Key Takeaways ---
// - std::async allows non-blocking execution of functions.
// - std::future provides a way to get the result of an asynchronous operation.
// - .get() on a future retrieves the result and can block the current thread.
// - Asynchronous operations are crucial for responsive UIs and efficient server applications.