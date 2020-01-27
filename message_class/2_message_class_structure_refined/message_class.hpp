#ifndef _MESSAGE_CLASS
#define _MESSAGE_CLASS

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <atomic>

#include <message_header_class.hpp>

/**
Message class inherits the header class and its management functions for tcp processing. \n
and adds the management functions for the data buffer. \n
The derived classes implement their specific data container and are responsible for initializing \n
the class members correctly, ie setting all header data as well preparing the data buffer \n
so that data length and data ptr are correct \n
*/

class message_class:public message_header_class{

private:
    //! global increment, each constructor call increases the counter
    static std::atomic<unsigned int> m_message_counter;

protected:
    // data length is included in header
    char *m_data_ptr;   // the ptr to the data, initialized to nullptr

public:
    //! default message creates empty message with unique message id
    message_class();
    //! default destructor
    ~message_class();

    //! this function fills the data buffer with the class's data
    virtual void prepare_data_buffer()=0;
    //! this function extracts the data
    virtual void extract_data_buffer()=0;
    //! gets the data length, used for tcp send to transfer to the data
    unsigned int get_data_length();
    //! gets the data pointer, used for tcp send to transfer to the data
    char* get_data_buffer_ptr();

    //! not sure this is necessary, used for testing so far
    unsigned int get_id();

    // move paradigm
    //message_class(message_class& other) = delete;
    //message_class& operator= (message_class& other) = delete;
    //message_class(message_class&& other) = default;
    //message_class& operator= (message_class&& other) = default;
};

#endif
