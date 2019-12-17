// mutex example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex

#include <unistd.h>         // usleep

#include <time.h>       /* time */
#include <stdlib.h>     /* srand, rand */


//#define Wmutex

int global_target_count = 10;

class controller{
private:
    static int counter;
    std::mutex controller_mutex;

public:
    controller(){ counter = 0; };
    void add(int n) {
        #ifdef Wmutex
        std::lock_guard<std::mutex> lck (controller_mutex);
        //controller_mutex.lock();
        #endif

        int current = counter;
        int delay = rand() % 10000;
        usleep(delay);  
        counter = current + n;
    
        //counter+=n;
        
        #ifdef Wmutex
        //controller_mutex.unlock();
        #endif

        printf("add %i   current now %i\n", n,counter);
    }
    void sub(int n) {
        #ifdef Wmutex
        std::lock_guard<std::mutex> lck (controller_mutex);
        //controller_mutex.lock();
        #endif

        int current = counter;
        int delay = rand() % 10000;
        usleep(delay);  
        counter = current - n;

        //counter-=n;
        #ifdef Wmutex
        //controller_mutex.unlock();
        #endif
        printf("sub %i   current now %i\n", n,counter);
    }
    int get() { return counter;};
};

int controller::counter = 0;
controller *global_ptr_controller = new controller();

void writer(int n, unsigned int t) {
    while (global_ptr_controller -> get() < global_target_count) {
        // critical section (exclusive access to std::cout signaled by locking mtx):
        //mtx.lock();

        global_ptr_controller->add(n);
        //mtx.unlock();
        usleep(t);
    }
}

void reader(int n, unsigned int t) {
    while (global_ptr_controller -> get() < global_target_count) {
        // critical section (exclusive access to std::cout signaled by locking mtx):
        //mtx.lock();
        global_ptr_controller->sub(n);
        //mtx.unlock();
        usleep(t);
    }
}

int main ()
{
    std::cout << "concurrency example : 2 each reader and writer processes" << std::endl;
    std::cout << "processes increase/decrease a global counter at different frequencies and with different increments" << std::endl;
    std::cout << "implemented with a global controler with a class lockguard .." << std::endl;
    std::cout << "uncomment #define Wmutex to enable lockguard" << std::endl;

    srand (time(NULL));

    global_ptr_controller = new controller();

    std::thread th1 (writer,1,1000000);
    std::thread th2 (writer,3,1000000);
    std::thread th3 (reader,1,1000000);
    std::thread th4 (reader,1,1500000);

    int current = global_ptr_controller -> get();

    while (current < global_target_count) {
        printf("counter = %i\n",current); 
        usleep(1000000);
        current = global_ptr_controller -> get();
    }

    printf("final counter = %i\n",current); 
    th1.join();
    th2.join();
    th3.join();
    th4.join();

    return 0;
}

