// conditionVariableFixed.cpp

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>

// base, coordination between data preparation and data consumption
// base is version 1, but going through several rounds of message passing instead of just one (implemented with 2 condvars
// base is version 2, here we add the global watcher thread which signals the end after N seconds, and passing on some real data

// not used yet
std::mutex message_mutex;
void message(std::string text){
    std::lock_guard<std::mutex> lck (message_mutex);
    std::cout << text << std::endl;
}
// not used yet
void message(std::string text, int value){
    std::lock_guard<std::mutex> lck (message_mutex);
    std::cout << text << value << std::endl;
}

std::time_t get_time(){
    // this seems to give millis
    //return std::chrono::system_clock::to_time_t( std::chrono::high_resolution_clock::now() );
    // this is nanos
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    return nanoseconds.count();
}

// a timer which  requests a shutdown after n seconds
std::mutex the_end_mutex;
bool the_end(false);
void watcher(int n) {
    message("creating watcher which starts the shutdown in sec = ",n);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000*n));
    //sleep(n);
    message("watcher thread starts shutdown");
    std::lock_guard<std::mutex> lck (message_mutex);
    the_end = true;
    //the_end.request_preparing_for_shutdown();
    //global_ptr_controller -> shut_down();
}

bool read_end_flag(){
    std::lock_guard<std::mutex> lck (message_mutex);
    bool result = the_end;
    return result;
}

const int work_count = 2;
std::time_t data;

std::mutex finish_mutex;
bool all_done(false);

std::mutex work_done_mutex;
std::condition_variable work_done_condvar;
bool data_ready(false);

std::mutex processing_done_mutex;
std::condition_variable processing_done_condvar;
bool processing_ready(false);

void waitingForWork(){

    while (true){
        // works with both scope bracket segments remmoved, see comments below
        // not sure why, becuase below the lockguard segement didn'twork unscoped
       {
        std::cout << "Worker: Waiting for work." << std::endl;
        std::unique_lock<std::mutex> work_lock(work_done_mutex);
        work_done_condvar.wait(work_lock,[]{return data_ready;});
        std::cout << "Worker: was notified data ready = " << std::endl;
    
        std::lock_guard<std::mutex> finish_lock(finish_mutex);
        std::cout << "Worker: checking global end flag." << std::endl;
        if (all_done == true) break;
        else std::cout << "data = " << data << std::endl;

        data_ready=false;
        std::cout << "Worker: Work done." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
       }
       {
        std::lock_guard<std::mutex> processing_lock(processing_done_mutex);
        processing_ready = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Worker: Processing done."  << std::endl;
        processing_done_condvar.notify_one();
       }
    }
    std::cout << "Worker: all done" << std::endl;
}

void setdata_ready(){
    int counter = 0;

    while (read_end_flag() == false){
    //while (counter < work_count){
        // work lock segment with lock_guard
        // removing scoped brackets will not work here, not sure why
        // removing the scope bracktes works when using the unique lock / unlock approach, not sure why
        {
        std::cout << "\nSender: round " << counter << std::endl;
        std::lock_guard<std::mutex> work_lock(work_done_mutex);
        //std::unique_lock<std::mutex> work_lock(work_done_mutex);
        data_ready=true;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        data = get_time();
        std::cout << "Sender: Data is ready."  << std::endl;
        //work_lock.unlock();
        work_done_condvar.notify_one();
        }
        // processing lock segment with unique lock
        // removing scoped brackets works
        {
        std::cout << "Sender: Waiting for processing done." << std::endl;
        std::unique_lock<std::mutex> processing_lock(processing_done_mutex);
        processing_done_condvar.wait(processing_lock,[]{return processing_ready;});
        processing_ready = false;
        std::cout << "Sender: was notified processing done" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
        counter++;
    }
    {
    std::lock_guard<std::mutex> finish_lock(finish_mutex);
    std::cout << "Sender: finishing up, all work is done" << std::endl;
    all_done = true;
    }
    std::lock_guard<std::mutex> lck(work_done_mutex);
    data_ready=true;
    work_done_condvar.notify_one();
    std::cout << "Sender: end notification sent" << std::endl;
}

int main(){

  std::cout << std::endl;

  std::thread watcher_thread(watcher,5);

  std::thread t1(waitingForWork);
  //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::thread t2(setdata_ready);

  t1.join();
  t2.join();
  watcher_thread.join();

  std::cout << std::endl;
  
}
