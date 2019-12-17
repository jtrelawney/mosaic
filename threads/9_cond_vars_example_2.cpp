// conditionVariableFixed.cpp

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>

const int work_count = 2;
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
        std::cout << "Worker: was notified data ready." << std::endl;
    
        std::lock_guard<std::mutex> finish_lock(finish_mutex);
        std::cout << "Worker: checking global end flag." << std::endl;
        if (all_done == true) break;
        data_ready=false;
        std::cout << "Worker: Work done." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
       }
       {
        std::lock_guard<std::mutex> processing_lock(processing_done_mutex);
        processing_ready = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cout << "Worker: Processing done."  << std::endl;
        processing_done_condvar.notify_one();
       }
    }
    std::cout << "Worker: all done" << std::endl;
}

void setdata_ready(){
    int counter = 0;
    while (counter < work_count){
        // work lock segment with lock_guard
        // removing scoped brackets will not work here, not sure why
        // removing the scope bracktes works when using the unique lock / unlock approach, not sure why
        {
        std::cout << "\nSender: round " << counter << std::endl;
        std::lock_guard<std::mutex> work_lock(work_done_mutex);
        //std::unique_lock<std::mutex> work_lock(work_done_mutex);
        data_ready=true;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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

  std::thread t1(waitingForWork);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::thread t2(setdata_ready);

  t1.join();
  t2.join();

  std::cout << std::endl;
  
}
