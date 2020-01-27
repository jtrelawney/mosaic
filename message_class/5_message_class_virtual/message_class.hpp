#ifndef MESSAGE_CLASS_H_
#define MESSAGE_CLASS_H_

#include <iostream>
#include <header_buffer.hpp>
#include <message_type_enum_class.hpp>
#include <time_class.hpp>

class buffer_info{
public:
    unsigned int buffer_length;
    char *buffer_pointer;
    bool valid;
    buffer_info():buffer_length(0),buffer_pointer(nullptr),valid(false){};
    ~buffer_info(){};
    void set_info(unsigned int length, char *ptr){buffer_length=length;buffer_pointer=ptr;valid=true;}
    void print(){
        std::cout << "buffer info - content" <<std::endl;
        std::cout << "valid = " << valid <<std::endl;
        std::cout << "length = " << buffer_length <<std::endl;
        std::cout << "char * = " << (void *) buffer_pointer <<std::endl;
    }
};

class message_class:protected header_buffer_class{

private:
    //! message type so that the proper object type can be reconstructed from buffer
    message_type_enum m_message_type;
    //! unique id for each message, best based on global counter
    unsigned int m_id;
    //! timestamp when the data was observed
    def_time_format m_sensor_time_stamp;

    //! default constructor not to be used
    message_class() = delete;

protected:
    //! copies the message class members into the buffer
    unsigned int populate_header_buffer();
    //! copies the buffer into the message class members
    unsigned int populate_member_data();//header_buffer_class &buffer);

public:
    //! construct with the private member settings
    message_class(message_type_enum mtype, unsigned int mid);
    //! construct with a given buffer (eg. after tcp header receiver)
    message_class(header_buffer_class &buffer);
    //! default de-structor
    ~message_class();

    //! populates the header buffer with the message class members by calling populate_header_buffer()
    virtual unsigned int populate_header_buffer_from_member_data();
    //! populates the message class members from the buffer by calling populate_member_data()
    virtual unsigned int populate_member_data_from_buffer(header_buffer_class &buffer);

    //! reads out the message type
    message_type_enum get_message_type();

    //! prints the message class member data
    virtual void print_meta_data();

    //! reads the header buffer parameters, since message class owns buffer this is final
    virtual buffer_info get_header_buffer_info() final;

    //! reads the data buffer parameters, since the derived class owns the data, this is pure virtual
    virtual buffer_info get_data_buffer_info()=0;
};

#endif
