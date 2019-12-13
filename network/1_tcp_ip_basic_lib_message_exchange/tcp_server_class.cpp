#include "tcp_server_class.h"

tcp_server_class::tcp_server_class(int port_number):tcp_class(port_number) {
    if (m_socket_fd<0) std::cout << "tcp server: constructor call failed" << std::endl;
    else {
        std::cout << "tcp server: constructor call succeeded" << std::endl;
        std::cout << "sever port no = " << m_port_no << std::endl;
        std::cout << "server socket fd = " << m_socket_fd << std::endl;
    }
}

tcp_server_class::~tcp_server_class(){
    std::cout << "destructor tcp server" << std::endl;
}
