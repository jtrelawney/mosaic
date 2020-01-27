#ifndef _MESSAGE_CLASS
#define _MESSAGE_CLASS

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <atomic>

const int TCP_HEADER_LENGTH = 12;

/**
basic message class which provides the mandatory members and functions for tcp processing
*/

class message_class{
private:
    static std::atomic<unsigned int> m_message_counter; // global increment, each constructor call increases the counter
protected:
    unsigned int m_id;  /// unique id for each message, based on counter

    char m_header_buffer[TCP_HEADER_LENGTH] = {0};  /// header buffer filled with all meta data, e.g. in preparation for tcp transmission
    unsigned int m_message_data_length;     /// the header length is fixed, the size of the message is variable

public:
    message_class(); /// creates empty message with unique message id
    ~message_class();

    unsigned int get_id();
    unsigned int get_message_data_length();

    // header management function
    // process at tcp receive: get the buffer ptr and let tcp fill itup, then extract the metadata into the members
    // process at tcp send: fill the buffer with metada and then hand the buffer ptr to tcp for transmission
    bool extract_header_buffer();   // given a buffer extract the meta data into the members
    bool prepare_header_buffer();   // given a complete message fill the buffer with metadata
    char* get_header_buffer_ptr();  // gets the buffer pointer, used for tcp to fill the header buffer

    void print_buffer_content();    /// to help debug

    // move paradigm
    //message_class(message_class& other) = delete;
    //message_class& operator= (message_class& other) = delete;
    //message_class(message_class&& other) = default;
    //message_class& operator= (message_class&& other) = default;
};

#endif
