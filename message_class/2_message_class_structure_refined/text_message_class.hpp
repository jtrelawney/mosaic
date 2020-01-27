#ifndef _TEXT_MESSAGE_CLASS
#define _TEXT_MESSAGE_CLASS

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <message_class.hpp>

class text_message_class:public message_class{

private:
    std::string m_message_text;

public:
    text_message_class(); /// creates empty message with unique message id
    text_message_class(std::string text);    /// creates message with text

    void set_data(std::string text);
    std::string get_data();

    void prepare_data_buffer();
    void extract_data_buffer();
    ~text_message_class();

    // move paradigm
    //message_class(message_class& other) = delete;
    //message_class& operator= (message_class& other) = delete;
    //message_class(message_class&& other) = default;
    //message_class& operator= (message_class&& other) = default;

};

#endif
