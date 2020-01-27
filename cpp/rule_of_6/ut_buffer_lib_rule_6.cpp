#include <header_buffer.hpp>
#include <iostream>

int main(){
    std::cout << "\n\ntesting constructors and assignments" << std::endl;
    std::cout << "\n\ncreate a" << std::endl;
    header_buffer_class a;
    std::cout << "\n\ncopy constructor b(a)" << std::endl;
    header_buffer_class b(a);
    std::cout << "\n\ncopy assignment c=a" << std::endl;
    header_buffer_class c = a;
    std::cout << "\n\nmove constructor d(move a)" << std::endl;
    header_buffer_class d(std::move(a));
    std::cout << "\n\nmove assignment e = move a" << std::endl;
    header_buffer_class e = std::move(a);

    std::cout << "\n\ncreate const a" << std::endl;
    const header_buffer_class ca;
    std::cout << "\n\ncopy constructor b(ca)" << std::endl;
    header_buffer_class cb(ca);
    std::cout << "\n\ncopy assignment cc=ca" << std::endl;
    header_buffer_class cc = ca;
    std::cout << "\n\nmove constructor cd(move ca)" << std::endl;
    header_buffer_class cd(std::move(ca));
    std::cout << "\n\nmove assignment ce = move ca" << std::endl;
    header_buffer_class ce = std::move(ca);
}
