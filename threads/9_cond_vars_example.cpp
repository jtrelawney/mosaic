// conditionVariableFixed.cpp

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>

const int delay = 2000;


std::mutex work_ready_mutex;
std::condition_variable work_ready_condVar;
bool dataReady(false);

std::mutex work_done_mutex;
std::condition_variable work_done_condVar;
bool dataDone(false);

std::mutex done_mutex;
bool allwork_done(false);

void doTheWork(){
  std::cout << "Processing shared data." << std::endl;
}

void waitingForWork(){

    
    bool completed = false;
    while (completed == false){

        //std::unique_lock<std::mutex> work_ready_lock(work_ready_mutex);
            std::lock_guard<std::mutex> work_ready_lock(work_ready_mutex);
            std::cout << "Worker: have work ready lock, release it and wait for signal" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            work_ready_condVar.wait(work_ready_lock,[]{return dataReady;});
    
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        std::cout << "Worker: released work ready lock"  << std::endl;
        doTheWork();
        dataReady=false;
        std::cout << "Work done." << std::endl;


        std::unique_lock<std::mutex> work_done_lock(work_done_mutex);
        dataDone=true;
        work_done_condVar.wait(work_done_lock,[]{return dataDone;});
        
    
/*    
        {
            std::lock_guard<std::mutex> done_lck(done_mutex);
            completed = allwork_done;
        }
        //work_ready_lock.unlock();
*/
    }


}

const int i = 2;
void setDataReady(){

    for (int j=0; j<i; j++){

        // this grabs the ready lock and signals ready
        std::unique_lock<std::mutex> work_ready_lock(work_ready_mutex);
        std::cout << "Sender: have work ready lock"  << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        dataReady=true;
        std::cout << "Sender: Data is ready, notify worker"  << std::endl;
        work_ready_condVar.notify_one();
        std::cout << "Sender: released work ready lock"  << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));        

        // this grabs the done lock and signals ready
        std::unique_lock<std::mutex> work_done_lock(work_done_mutex);
        std::cout << "Sender: have work done lock"  << std::endl;
        work_done_condVar.wait(work_done_lock,[]{return dataDone;});
        std::cout << "Sender: Data is processed, create more work"  << std::endl;
        dataDone=false;

        //std::cout << "Sender: released work done lock"  << std::endl;
        
        //std::cout << "Sender: aha work was completed"  << std::endl;
        //work_ready_condVar.notify_one();

    //    work_ready_lock.lock();
/*
        std::cout << "Sender: obtaining work_done_lock"  << std::endl;
        std::unique_lock<std::mutex> done_lock(work_done_mutex);
        std::cout << "Sender: work_done_lock obtained, now waiting"  << std::endl;
        work_done_condVar.wait(done_lock,[]{return dataDone;});
        
*/
    }

    std::unique_lock<std::mutex> done_lck(done_mutex);
    allwork_done = true;
    work_ready_condVar.notify_one();
}

int main(){

  std::cout << std::endl;

  std::thread t1(waitingForWork);
  std::thread t2(setDataReady);

  work_done_condVar.notify_one();

  t1.join();
  t2.join();

  std::cout << std::endl;
  
}
