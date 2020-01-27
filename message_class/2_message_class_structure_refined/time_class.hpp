#ifndef _TIME_CLASS
#define _TIME_CLASS

#include <chrono> // for timing stuff

using def_time_format = std::time_t;

def_time_format get_time_stamp_ns(){
    // this seems to give millis
    //return std::chrono::system_clock::to_time_t( std::chrono::high_resolution_clock::now() );
    // this is nanos
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    return nanoseconds.count();
}

#endif
