#ifndef _message_header_class
#define _message_header_class

#include <cstring> // for memcpy
#include <iostream> // for cout
#include <assert.h> // for assert

#include <time_class.hpp>

//! the header length is fixed
const int TCP_HEADER_LENGTH = 16;

/**
This class defines the message header members common for all messages. \n
It provides the constructors to fill the header info and keeps a buffer. \n
It also provides the functions to fill the buffer from the members and populate the members from a given buffer \n
All header members will be preserved during the TCP send, receive and message reconstruction process. \n
*/

class message_header_class{
private:

    //! header buffer of fixed length, use populate_buffer_from_member_data() to transfer member data into buffer
    char m_header_buffer[TCP_HEADER_LENGTH] = {0};

protected:
    //! unique id for each message, best based on global counter
    unsigned int m_id;
    //! the size of the data buffer depends on the message type and is updated by the derived class
    unsigned int m_data_length;
    //! timestamp when the data was observed
    def_time_format m_sensor_time_stamp;

public:
    //! default constructor, used to create empty message, e.g after TCP receive, then fill with header info
    message_header_class();
    //! constructor to use, sets all known members
    message_header_class(unsigned int id);
    //! destructor, no resources to be freed manually
    ~message_header_class();
    //! prints the header data
    void print_header_data();
    //! prints the content of the header buffer
    void print_header_buffer_content();

    //! provides the header buffer length
    unsigned int get_header_length();
    //! provides the header buffer pointer, used for TCP send
    char* get_header_buffer_ptr();
    //void print_header_buffer_content();

    //! copies the member information into the buffer, used for TCP send
    void populate_buffer_from_member_data();
    //! copies the buffer data into the members, used after TCP receive to reconstruct the message
    void populate_member_data_from_buffer();
};

#endif
