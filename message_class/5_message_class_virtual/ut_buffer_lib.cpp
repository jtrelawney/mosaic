#include <header_buffer.hpp>
#include <iostream>

void read_buffer(header_buffer_class &buffer){
    unsigned int id;
    buffer.get_buffer_data(id,0);
    float k;
    buffer.get_buffer_data(k,sizeof(unsigned int));
    std::cout << "reading id = " << id << "   k = " << k << std::endl;
}

void write_buffer(header_buffer_class &buffer){
    unsigned int id = 5;
    float k = 3.14;
    std::cout << "writing id = " << id << "   k = " << k << std::endl;
    buffer.set_buffer_data(id,0);
    buffer.set_buffer_data(k,sizeof(unsigned int));
}

int main(){
    header_buffer_class buffer;
    write_buffer(buffer);
    read_buffer(buffer);

    std::cout << "writing with improper buffer pos" << std::endl;
    unsigned int id = 5;
    unsigned int pos = buffer.get_buffer_length()-sizeof(id);
    std::cout <<"pos = " << pos << std::endl;
    buffer.set_buffer_data(id,pos-1);
    buffer.print_header_buffer_content();
    std::cout << "reading with improper buffer pos" << std::endl;
    buffer.get_buffer_data(id,pos);
}
