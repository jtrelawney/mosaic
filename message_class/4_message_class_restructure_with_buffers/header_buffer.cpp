#include<header_buffer.hpp>


header_buffer_class::header_buffer_class():m_buffer(std::string(TCP_HEADER_LENGTH,0)){
    std::cout << "header_buffer_class constructor" << std::endl;
}

header_buffer_class::~header_buffer_class(){
    std::cout << "header_buffer_class de-structor --------------------" << std::endl;
}

unsigned int header_buffer_class::get_buffer_length() const {
    return TCP_HEADER_LENGTH;
}

char *header_buffer_class::get_buffer_ptr(){
    return const_cast<char*>(&*m_buffer.begin());
}

void header_buffer_class::print_header_buffer_content(){
    std::cout << "Header - Buffer - content = ";
    unsigned int buffer_length = get_buffer_length();
    char *buffer_ptr = get_buffer_ptr();
    for (unsigned int i = 0; i<buffer_length; i++) {
        std::cout << (int)(*buffer_ptr) <<",";
        buffer_ptr++;
    }
    std::cout << std::endl;
}
