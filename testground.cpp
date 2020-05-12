//Create a group of C++11 threads from the main program

#include <iostream>
#include <thread>

static const int num_threads = 10;

//This function will be called from a thread

void call_from_thread(int x) {
    std::cout << "Launched by thread\n";
}

int main() {
    std::thread t;

    //Launch a group of threads
    for (int i = 0; i < num_threads; ++i) {
        t = std::thread(call_from_thread,0);
    }

    std::cout << "Launched from the main\n";

    //Join the threads with the main thread
    for (int i = 0; i < num_threads; ++i) {
        t.join();
    }

    return 0;
}
