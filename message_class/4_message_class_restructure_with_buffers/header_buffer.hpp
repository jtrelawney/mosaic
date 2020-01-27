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
    //! the header length is fixed
    static const int TCP_HEADER_LENGTH = 17;

    //! the buffer is implemented as a fixed length string
    std::string m_buffer;

public:
    //! default constructor, creates the string buffer
    header_buffer_class();
    //! default de-structor, empty
    ~header_buffer_class();

    //! provide buffer length
    unsigned int get_buffer_length() const;
    //! provide buffer ptr, to allow modification not as const, maybe overload subscript for safety
    char *get_buffer_ptr();

    //! prints the content of the header buffer
    void print_header_buffer_content();
};

#endif
