// mutex example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <condition_variable>
#include <chrono>
#include <unistd.h>         // usleep
#include <queue>         // queue


std::mutex message_mutex;
void message(std::string text){
    std::lock_guard<std::mutex> lck (message_mutex);
    std::cout << text << std::endl;
}

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

class controller;
static controller *global_ptr_controller;

const int N_readers(10);
//const int N_readers(6);

// mutexes for getting the locks
std::mutex message_available_mutex[N_readers];
// cond vars to signal the wakeups
std::condition_variable message_available_condition[N_readers];
// locks cannot be global, this leads to issues at shutdown if an unlock happens on an already unlocked lock

// implement a global end flag
class end_flag_class {
public:
    enum status_list {
        ongoing=0,                  // status at start, normal processing
        preparing_for_shutdown=1,   // the watcher requests this status when the timer runs out
                                    // - the writer threads will stop queuing new messages
                                    // - the reader threads will continue to dequeue, but stop queuing requests
                                    // - the controller will reject new queue requests, but will continue to serve dequeue requests until the queue is empty
                                    //   then the controller will request ready_for_shutdown()
        ready_for_shutdown=2,       // the controller will request this status when the message queue has been emptied during shutdown
                                    // it will then wake all readers who see the status and terminate
        shutdown_complete=3         // will be requested by the main program when the processes have finished the join, not necessary but a final touch
    };

private:
    std::mutex flag_mutex;
    bool end_flag;
    status_list end_flag_status;

public:
    end_flag_class():end_flag(false),end_flag_status(ongoing){ message("constructing end flag , set to false"); };
    ~end_flag_class(){ message("destructing end flag"); };
    
    void request_preparing_for_shutdown() {
        message("setting end_flag state to: preparing_for_shutdown");
        std::lock_guard<std::mutex> lck (flag_mutex);
        end_flag_status = preparing_for_shutdown;
    }
    void request_ready_for_shutdown() {
        message("setting end_flag state to: ready_for_shutdown");
        std::lock_guard<std::mutex> lck (flag_mutex);
        end_flag_status = ready_for_shutdown;
    }
    void request_shutdown_complete() {
        message("setting end_flag state to: shutdown_complete");
        std::lock_guard<std::mutex> lck (flag_mutex);
        end_flag_status = shutdown_complete;
    }

    status_list read_status(){ return end_flag_status;};
};

// on global end flag class
static end_flag_class the_end;

class controller{
private:
    int id;
    std::mutex id_mutex;
    std::mutex controller_mutex;
    bool shut_down_flag;

    std::queue<int> target_ids_q;
    std::queue<int> message_q;

    // only call when the data lock has been obtained, so that notify_one() can pass it on
    // or is it the controller flag
    void notify_first(){
        if (!message_q.empty()) {
            int next_id = target_ids_q.front();
            //std::cout << "q notifies r = " << next_id << std::endl;
            message("q notifies r = ",next_id);
            message_available_condition[next_id].notify_one();
        }
    }

public:
    controller(): id(0),shut_down_flag(false){};
    virtual ~controller(){ message("q destructor");};
    int next_id(){
        std::lock_guard<std::mutex> lck (id_mutex);
        id++;
        return id;
    }

    // the watcher calls this function after settig the status to preparing_for_shutdown
    void shut_down() {
        std::time_t ct = get_time();
        message("shutdown requested @ ",ct);
           
        std::lock_guard<std::mutex> lck (controller_mutex);

        // ignore duplicated shutdown requests, shouldn't happen because the watcher only calls it once
        if (shut_down_flag == true){
            message("q shutdown already in progress");
        } else {
            // set own shutdown flag so that new queuing requests can be rejected and de_queue requests can continue
            shut_down_flag = true;

            while (!is_empty()) {
                message("queue not empty : notify first");
                notify_first();
                // get a lock
                //std::lock_guard<std::mutex> lck (controller_mutex);
                //usleep(10000);
            }
            //the_end.request_ready_for_shutdown();
            // then notify the first process in the message queue to start the depletion process
            //notify_first();
        }

            // little problem: we don't own the lock, so choose controlled shutdown with waits 
            /*
            while (!is_empty()) {
                notify_first();
                usleep(10000);
            }
            */
        
            /*    
            //message("queue is empty, set flag and wake everybody");
            if (!is_empty()) notify_first();
            
            // once the queue is empty set the flag and wake all sleeping processes so that they can see it and exit
            message("queue is empty, set flag and wake everybody");
            //std::cout << "queue is empty, set flag and wake everybody" << std::endl;
            {
                
                shut_down_flag = true;
            }


            message("shutdown waking all process to ensure they see the end flag");
            for (int i=0; i<N_readers; i++){
                //std::unique_lock<std::mutex> lock_it(message_available_mutex[reader_index]);
                message("notify reader = ", i);
                message_available_condition[i].notify_one();
            }
        }
        */
    };

    void enqueue(int n, int target_id) {

        std::time_t ct = get_time();
        std::string text = "request to q : n = " + std::to_string(n) + " for " + std::to_string(target_id) + " @ " + std::to_string(ct);
        message(text);

        // get a lock
        std::lock_guard<std::mutex> lck (controller_mutex);

        // unless shutdown is in progress queue the request and inform the first message in the queue to read his message
        if (shut_down_flag == true) {
            text = "shutdown in progress ... drop this message message";
            message(text);
        } else {
            message("q gets lock and records n = ",n);
            message_q.push(n);
            target_ids_q.push(target_id);
            notify_first(); //TODO review when the notify is necessary to keep the emptying of the queue going
        }
    }

    int dequeue(int target_id) {
        std::time_t ct = get_time();
        std::string text = "request to de-q for r = " + std::to_string(target_id) + " @ " + std::to_string(ct);
        message(text);

        // get a lock on the data
        std::lock_guard<std::mutex> lck (controller_mutex);
        bool isempty = message_q.empty();
        // this means the queue is empty and the requester was wrong to deqeue
        // it shouldn't happen, but if a requester wants to read from empty queue then it is a problem
        bool just_empty = ( ( isempty == true) && (shut_down_flag == false) );
        // this means the queue is empty during shutdown
        // writers have terminated and readers have stopped queueing messages, hence this means that the shutdown is complete
        bool shutdown_empty = ( ( isempty == true) && (shut_down_flag == true) );

        if ( just_empty == true){
            std::string text = "message q is empty, request for r = " + std::to_string(target_id) + " invalid";
            message(text);            
            return -2;
        }

        if ( shutdown_empty == true) {
            std::string text = "message q is empty, and shutdown flag is in progress -> we are done";
            message(text);
            the_end.request_shutdown_complete();
            return -3;
        }

        // at this point the exceptions have been worked through and it is normal mode
        // prepare data and target
        int n, stored_target_id = target_ids_q.front();
        // return the data and update the queue if data is for the requester  
        if (stored_target_id == target_id){
            target_ids_q.pop();
            n = message_q.front();
            message_q.pop();
            // at this point the queues are consistent, both the target and the data queue have been poped
            std::string text = "q  message is for r = " + std::to_string(stored_target_id) + " n = " + std::to_string(n);
            message(text);
            // but if the queue is still not empty then the next message needs to be processed too
            notify_first();
        } else
        // if data is not for requester then inform the owner
        {
            std::string text = "q  message is not for r = " + std::to_string(target_id) + " but for r = " + std::to_string(stored_target_id) + " , notify";
            message(text);
            message_available_condition[stored_target_id].notify_one();
            return -1;            
        }
        return n;        
    }

    int get_size() {
        std::lock_guard<std::mutex> lck (controller_mutex);
        int counter = message_q.size();
        return counter;
    };

    bool is_empty() {
        std::lock_guard<std::mutex> lck (controller_mutex);
        return message_q.empty();
    };

    bool get_shut_down_flag() {
            std::lock_guard<std::mutex> lck (controller_mutex);
            return shut_down_flag;
    };
};

int N_writers = 3;

int get_random_target(int reader_index){ int result=0; while( (result < N_writers) || (result == reader_index) ) result = std::rand() % (N_readers); return result;  }

class reader_class{
private:
    int reader_index;
    int write_freq;
    bool writer;
public:
    reader_class():reader_index(-1){ message("simple reader constructor"); };

    virtual ~reader_class(){ if (writer == true) message("writer de-structor ",reader_index); else message("reader de-structor ",reader_index); };
    
    reader_class(int index): reader_index(index),write_freq(0),writer(false){
        write_freq = 100000/N_readers;
        std::string text = "reader " + std::to_string(reader_index) + " write freq = " + std::to_string(write_freq);
        if (index < N_writers) { writer=true; text+= ", turning writer";  }
        message(text);
    };

    // this is the funcion each reader/writer thread keeps calling until the shutdown is complete
    // writer threads keep queuing new message until they see the shutdown requested flag, then they retire
    void read(){
        std::string text;
        std::time_t ct;

        // distinguish between writer and reader threads

        // writer section
        if (writer == true){

            // as long as the end_flag_status permits, keep queuing new messages
            while (the_end.read_status() == end_flag_class::status_list::ongoing) {
                int id = global_ptr_controller -> next_id();
                int target_id = get_random_target(reader_index);
                text = "writer " + std::to_string(reader_index) + " requests " + std::to_string(id) + " to " + std::to_string(target_id);
                message(text);
                global_ptr_controller->enqueue(id,target_id);
                usleep(write_freq);
            }
            // watcher has signaled shutdown request, writer processes are done
            ct = get_time();
            text = "writer " + std::to_string(reader_index) + " is stops queuing message @ " + std::to_string(ct);
            message(text);
            //global_ptr_controller -> shut_down();
        }

        // reader section
        else {
            // get lock then in loop sleep until notified
            text = "r = " + std::to_string(reader_index) + " locks its message_available_mutex";
            message(text);
            std::unique_lock<std::mutex> lock_it(message_available_mutex[reader_index]);

            // keep reading messages
            while(true) {
                text = "r = " + std::to_string(reader_index) + " starts waiting for its message_available_condition";
                message(text);

                // wait for condition
                message_available_condition[reader_index].wait(lock_it);
                ct = get_time();
                text = "r = " + std::to_string(reader_index) + " wakes up @ " + std::to_string(ct);
                message(text);

                // check endflag status and if ready_for_shutdown (message queue is empty) then exit
                end_flag_class::status_list end_flag_status = the_end.read_status();
                if ( end_flag_status == end_flag_class::status_list::ready_for_shutdown) break;

                // if ongoing then first read the message
                int result = global_ptr_controller->dequeue(reader_index);
                text = "r = " + std::to_string(reader_index) + " reads " + std::to_string(result) + " from q";
                message(text);

                switch(result){
                    case -1:
                        text = "message not for r = " + std::to_string(reader_index) + " request ignored";
                        message(text);
                        break;
                    case -2:
                        text = "message q is empty, r = " + std::to_string(reader_index) + " reqest ignored";
                        message(text);  
                        break;
                    case -3:
                        text = "message q is empty and shutdown in progress, r = " + std::to_string(reader_index) + " checking for shutdown";
                        message(text);
                        break;
                }

                // if ready_for_shutdown then ext
                //if ( end_flag_status == end_flag_class::status_list::preparing_for_shutdown) break;
                        
                // in normal operation also respond with some random result sometimes
                if ( end_flag_status == end_flag_class::status_list::ongoing) {
                    int rand_response = std::rand() % 10;
                    if (rand_response>2) {
                        int time_var = std::rand() % 1000+500;
                        text = "r = " + std::to_string(reader_index) + " becomes busy for " + std::to_string(time_var);
                        message(text);
                        usleep(time_var);

                        int id = global_ptr_controller -> next_id();
                        int target_id = get_random_target(reader_index);
                        text = "r = " + std::to_string(reader_index) + " responds with sending n = " + std::to_string(id) + " to  r = " + std::to_string(target_id);
                        message(text);

                        global_ptr_controller->enqueue(id,target_id);
                        usleep(write_freq);
                    } // random response
                } // ongoing -> respond
            }// while loop for reader

            ct = get_time();
            text = "r = " + std::to_string(reader_index) + " verified the shutdown flag and is exiting @ " + std::to_string(ct);
            message(text);
        } // reader section
    };// read function
}; // reader_class

// a timer which  requests a shutdown after n seconds
void watcher(int n) {
    message("creating watcher which starts the shutdown in sec = ",n);
    sleep(n);
    message("watcher thread starts shutdown");
    the_end.request_preparing_for_shutdown();
    global_ptr_controller -> shut_down();
}

int main ()
{
    printf("the main creates a global message controller queue and several readers one of which is a 'writer' process\n");
    printf("the 'writer process' regularly queues a message to a random target reader and does not receive message itself\n");
    printf("all other reader proocess sometimes create a response to a random other reader\n");
    printf("the processes run for SEC secs, a watcher thread sets the global the_end flag to true\n");

    printf("the main watches the global end flag and once it occurs it calls the message queue shutdown function\n");
    printf("it then joins the terminating processes and afterwards deletes the message controller\n");

    printf("the global controller manages the message queue, all operations are implemented with a lockguard\n");
    printf("new messages are queued and then for the intended receipient of the first messgage in the queue the condition variable is notified\n");
    printf("each reader has its own condition variable (array)  and the target id is used as the index for the condition variable array\n");
    printf("when a reader tries to read the first element in the queue and it is not for the reader, -1 is returned\n");
    printf("and the condition variable of the target id is called\n");
    printf("the controller is not a process rather a global object - so it doesn't monitor the the_end-flag\n");
    printf("it provides a shutdown function which the main calls to end the process\n");
    printf("in the shutdown function the controller notifies the reader for the first message in the qeueu, until\n");
    printf("the queue is empty. Then the controller sets teh shutdonw flag and notifies all readers so that they can see the shutdown flag is set and terminate\n");

    printf("message ids are created sequentially by the controller, so they are unique but they maybe queued out of order or displayed out of order\n");
    printf("since the shutdown is synchronized all messages should be completely printed\n");

    printf("each reader has a unique integer id which is used to identify its condition variable and allows to select the correct messages\n");
    printf("when the readers gets notified and holds the lock, it first checks the shutdown flag and starts the exit process if that is the case\n");
    printf("in its loop the reader dequeues the next massage, it can assume it is for him since it was notified. if this is not the case -1 is returned\n");
    printf("if this element is for the reader the value is reutrned and the element is removed from the queue\n");
    printf("the controller ensures that the queue will be emptied at shutdown, so it will notify the process to read their remaining message\n");
    printf("if during this process a reader wants to send a response it will be rejected (at this point the shutdown flag is not yet set)\n");
    printf("when the shutdown flag is set the queue will have been emptied so when the reader sees the flag it can exit\n");

    printf("compilation with the debug and threadsnaitizer, if the interprocess communication is wrong warnings will be issued\n");
    printf("run with ./7_queue_mpmc_cond_vars > test.txt 2>&1 /  to catch the warnings");
    printf("to test run with ./7_queue_mpmc_cond_vars > test.txt    then compare\n");
    printf("cat test.txt | grep q ing \n");
    printf("cat test.txt | grep reads | grep -v -1\n");

    global_ptr_controller = new controller();

    const int SEC = 3;
    
    // run for a certain amount of time
    std::thread watcher_process(watcher,SEC);
   
    std::vector<std::thread> reader_process;
    for (int i=0; i<N_readers; i++) {
        message("starting reader ",i);
        // this init creates some weird destructor calls probably one for the intiializing the vector entry, then destroy, then one for this and oen destroy ....
        reader_process.push_back(std::thread(&reader_class::read,reader_class(i)) );
    }

    // now continue to give status updates every second
    int current_queue_size;
    while (the_end.read_status() != end_flag_class::status_list::ready_for_shutdown) {
        usleep(1000000);
        current_queue_size = global_ptr_controller -> get_size();
        message("status counter = queue size = ",current_queue_size);
    }

    current_queue_size = global_ptr_controller -> get_size();
    message("final status counter = ",current_queue_size);
    //std::cout << "final status counter = " << current_queue_size << std::endl;

    watcher_process.join();
    for (int i = 0; i < N_readers; ++i) {
        reader_process[i].join();
    }
    message("processes terminated");
    delete global_ptr_controller;

    message("controller done too");
    return 0;
}

