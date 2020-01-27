#ifndef _message_header_class
#define _message_header_class

#include <cstring> // for memcpy
#include <iostream> // for cout
#include <assert.h> // for assert
#include <algorithm>    // std::copy

#include <time_class.hpp>
#include <header_buffer.hpp>
#include <message_type_enum_class.hpp>


/**
This other_data_class is a simulation for a data object, which will be handed over \n
to the message class to be processed. The Message class will take ownership for example an opencv::mat \n
and then transfer the data via TCP \n
To access the data a data ptr and the data length is required. \n
*/

class other_data_class{
private:
    std::string m_text;
public:
    other_data_class():m_text("test_data"){std::cout << "constructor : other_data, d* = " << (void *) get_data_ptr() << std::endl;};
    other_data_class(unsigned int l):m_text(std::string(l,'x')){std::cout << "constructor : other_data l=" << l << " text = " << m_text << "   d* = " << (void *) get_data_ptr() << std::endl;};
    unsigned int get_data_length(){ return m_text.length();};
    char *get_data_ptr(){return &*m_text.begin();};
    std::string get_data(){ return m_text;};
};


/*
This message class is the interface between the sensor/processing and tcp. \n
The sensor creates data and hands over ownership to the message class to send it via TCP \n
For processing TCP receives the data and creeates a message, when then hands it over \n
For each of the data object the message class provides a constructor which takes\n
of the data, creates the header data and provides buffer access to the data for tcp\n
The message class also provides a constructor for a given header buffer. This \n
constructor extracts the header info and creates an empty data object which in turn \n
provides the data ptr to fill the data from tcp into */

class message_class{
private:

    //! message type so that later can be determined which data object is valid
    message_type_enum m_message_type;

    //! each possible data object has its constructor and get function
    other_data_class m_other_data;

    //! default constructor, used to create empty message, e.g after TCP receive, then fill with header info
    message_class();

    //! copies the buffer data into the members, used after TCP receive to reconstruct the message
    void populate_member_data_from_buffer(header_buffer_class &buffer);

protected:
    //! ptr to the data container, used by tcp to transfer / received data
    char *m_data_ptr;
    //! unique id for each message, best based on global counter
    unsigned int m_id;
    //! the size of the data buffer depends on the message type and is updated by the derived class
    unsigned int m_data_length;
    //! timestamp when the data was observed
    def_time_format m_sensor_time_stamp;

public:

    //message_header_class(unsigned int id);
    // constructor to use, sets all known members
    //message_class(unsigned int id, message_type_enum message_type);

    //! constructor for the test data class
    message_class(unsigned int id, other_data_class data);
    //! exstractor function for the test data class
    other_data_class get_od_data();

    //! constructor to use to create message from given buffer (after tcp receive)
    message_class(header_buffer_class &buffer);

    //! destructor, no resources to be freed manually
    ~message_class();

    //! prints the header data
    void print_meta_data();
    //! prints the data
    void print_data();
    //! prints the data buffer (as int)
    void print_data_buffer(const unsigned int count);

    //! copies the member information into the buffer, used for TCP send
    header_buffer_class populate_buffer_from_member_data();

    //! provides buffer length for the tcp send process
    unsigned int get_data_length();
    //! provides buffer ptr for the tcp send process
    char *get_data_ptr();

    //! to find out which data object is in the message
    message_type_enum get_message_type();
};

#endif
