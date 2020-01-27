#include <cstring> // for memcpy
#include <iostream> // for cout
#include <assert.h> // for assert


#include <header_buffer.hpp>

int main(int argc, char *argv[])
{
    header_buffer_class hb;
    unsigned int buffer_length = hb.get_buffer_length();
    const char *buffer_ptr = hb.get_buffer_ptr();
    std::cout << "buffer length = " << buffer_length << std::endl;
    std::cout << "buffer ptr = " << (void *)buffer_ptr << std::endl;
    //buffer_ptr[3] = 'z';
    std::cout << "buffer content = " << std::endl;
    for (unsigned int i=0; i<buffer_length; i++){
        std::cout << (int)*(buffer_ptr+i) << ",";
    }
    std::cout << std::endl;
    return 0;
}
