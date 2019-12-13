#ifndef _MESSAGE_CLASS
#define _MESSAGE_CLASS

#include <iostream>
#include <cstring>
#include <stdio.h>

class message_class{
private:
    std::string m_message_text;
    unsigned int m_id;
    message_class();
public:
    //message_class(std::string text);
    message_class(std::string text, unsigned int id);
    std::string get_text();
    ~message_class();


    // move paradigm
    //message_class(message_class& other) = delete;
    //message_class& operator= (message_class& other) = delete;
    //message_class(message_class&& other) = default;
    //message_class& operator= (message_class&& other) = default;

};

#endif
