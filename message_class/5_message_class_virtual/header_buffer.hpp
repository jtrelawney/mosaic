#ifndef _header_buffer_class
#define _header_buffer_class

#include <cstring> // for memcpy
#include <iostream> // for cout
#include <assert.h> // for assert

/**
The header buffer class provides is the interface to the tcp class.\n
The header buffer is implemented as a fixed size string and it provides\n
the char *to the string and its length as key functions, which used by the \n
tcp class which requires a char * buffer to the data and its length to send the data.\n
*/

class header_buffer_class{
private:
    //! the header length is fixed and not negotiable
    static const int TCP_HEADER_LENGTH = 26;

protected:
    //! the buffer is implemented as a fixed length string
    std::string m_buffer;

    char *m_pointer;

public:
    // rule of 6

    //! default constructor, creates the string buffer
    header_buffer_class();

    //! default de-structor, empty
    ~header_buffer_class();

    void set_buffer(header_buffer_class &buffer);

    //! provide buffer length
    unsigned int get_buffer_length() const;
    //! provide buffer ptr, to allow modification not as const, maybe overload subscript for safety
    char *get_buffer_ptr();

    //! copy the given data to the buffer at the startpostion
    template<typename T>
    unsigned int get_buffer_data(T &data, unsigned int start_position){
        // confirm valid buffer position
        unsigned int data_length = sizeof(data);
        assert (start_position + data_length < TCP_HEADER_LENGTH);

        // target position
        T *ptr = &data;

        // source position
        char *buffer_ptr = get_buffer_ptr();
        buffer_ptr+=start_position;

        std::copy( buffer_ptr, buffer_ptr + data_length , (char *)ptr );

        return data_length;
    }

    template<typename T>
    unsigned int set_buffer_data(T &data, unsigned int start_position){
        // confirm valid buffer position
        unsigned int data_length = sizeof(data);
        assert (start_position + data_length < TCP_HEADER_LENGTH);

        // source position
        T *ptr = &data;

        // target position
        char *buffer_ptr = get_buffer_ptr();
        buffer_ptr+=start_position;

        std::copy( (char *)ptr, (char *)ptr+data_length, buffer_ptr );

        return data_length;
    }

    //! prints the content of the header buffer
    void print_header_buffer_content();
};

#endif
