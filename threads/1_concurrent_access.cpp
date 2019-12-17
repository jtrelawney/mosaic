// mutex example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex

#include <unistd.h>         // usleep

std::mutex mtx;           // mutex for critical section


const int global_counter = 10;

class controller{
private:
    static int counter;
public:
    controller(){ counter = 0; };
    void add(int n) {
        int new_counter = counter + n;
        printf("adding %i   to current %i  , new %i\n", n,counter,new_counter);
        counter = new_counter;
    }
    void add_exclusive(int n) {
        mtx.lock();
        int new_counter = counter + n;
        printf("adding %i   to current %i  , new %i\n", n,counter,new_counter);
        counter = new_counter;
        mtx.unlock();
    }
    int get() { return counter;};
};

int controller::counter = 0;
controller *global_ptr_controller = new controller();


void writer(int n, unsigned int t) {
    std::thread::id pid = std::this_thread::get_id();
    int value = global_ptr_controller -> get();
    do {
        //critical section (exclusive access to get, add during wait
        // without it the threads have the wrong value
        //mtx.lock();
        value = global_ptr_controller -> get();
        if (value<global_counter){
            int new_value = value+n;
            //std::cout << "process "<<pid<<" reads " << value << " adds " << n << " expected new value = " << new_value << std::endl;
            usleep(t);
            std::cout << "process "<<pid<<" read earlier " << value << " now adds " << n << " expected new value = " << new_value << std::endl;
            global_ptr_controller->add(n);
        }
        //mtx.unlock();
        usleep(t);
    } while (value<global_counter);    
}



int main ()
{
    std::cout << "concurrency example : 2 processes increase a global counter at different frequencies and with different increments" << std::endl;
    std::cout << "implemented with a global controler with global mutex - lock .." << std::endl;

    int current = global_ptr_controller -> get();

    std::thread th1 (writer,1,500000);
    std::thread th2 (writer,2,2000000);    

    while (current < global_counter) {
        printf("counter = %i\n",current); 
        usleep(1000000);
        current = global_ptr_controller -> get();
    }

    printf("final counter = %i\n",current);
    printf("=================== output below is from processes who are active after the global counter has been reached\n"); 
    th1.join();
    th2.join();

    return 0;
}

