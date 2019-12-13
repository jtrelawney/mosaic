#ifndef _MESSAGE_CLASS
#define _MESSAGE_CLASS

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <tcp_lib.h>

class message_class{
private:
    static unsigned int m_message_counter;
    std::string m_message_text;

    unsigned int m_id;
    char m_header_buffer[TCP_HEADER_LENGTH] = {0};
    unsigned int m_message_data_length;

    void print_buffer_content();

public:
    message_class(); /// creates empty message with unique message id
    message_class(std::string text);    /// creates message with text

    // header management function
    bool extract_header_buffer();   // from the buffer create the meta data
    bool prepare_header_buffer();   // from the meta data fill the buffer
    char* get_header_buffer_ptr();  // gets the pointer, used for tcp to fill the header buffer

    void set_text(std::string text);
    std::string get_text();
    ~message_class();

    unsigned int get_message_data_length();


    // move paradigm
    //message_class(message_class& other) = delete;
    //message_class& operator= (message_class& other) = delete;
    //message_class(message_class&& other) = default;
    //message_class& operator= (message_class&& other) = default;

};

#endif
