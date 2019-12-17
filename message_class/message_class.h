#ifndef _MESSAGE_CLASS
#define _MESSAGE_CLASS

#include <iostream>
#include <cstring>
#include <stdio.h>

const int TCP_HEADER_LENGTH = 12;

class message_class{
private:
    static unsigned int m_message_counter; // global increment, each constructor call increases the counter
protected:
    unsigned int m_id;  /// unique id for each message, based on counter

    char m_header_buffer[TCP_HEADER_LENGTH] = {0};  /// header buffer filled with all meta data, e.g. in preparation for tcp transmission
    unsigned int m_message_data_length;     /// the header length is fixed, the size of the message is variable

    // will be deleted    
    //std::string m_message_text;

    void print_buffer_content();    /// to help debug

public:
    message_class(); /// creates empty message with unique message id
    //message_class(std::string text);    /// creates message with text

    unsigned int get_id();

    // header management function
    bool extract_header_buffer();   // from the buffer create the meta data
    bool prepare_header_buffer();   // from the meta data fill the buffer
    char* get_header_buffer_ptr();  // gets the pointer, used for tcp to fill the header buffer

    //void set_text(std::string text);
    //std::string get_text();
    ~message_class();

    unsigned int get_message_data_length();


    // move paradigm
    //message_class(message_class& other) = delete;
    //message_class& operator= (message_class& other) = delete;
    //message_class(message_class&& other) = default;
    //message_class& operator= (message_class&& other) = default;

};

#endif
