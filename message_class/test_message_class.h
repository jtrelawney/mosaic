#ifndef _TEST_MESSAGE_CLASS
#define _TEST_MESSAGE_CLASS

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <message_class.h>

class test_message_class:public message_class{

private:
    std::string m_message_text;

public:
    test_message_class(); /// creates empty message with unique message id
    test_message_class(std::string text);    /// creates message with text

    void set_data(std::string text);
    std::string get_data();

    ~test_message_class();

    // move paradigm
    //message_class(message_class& other) = delete;
    //message_class& operator= (message_class& other) = delete;
    //message_class(message_class&& other) = default;
    //message_class& operator= (message_class&& other) = default;

};

#endif
