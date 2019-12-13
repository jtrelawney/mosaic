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

std::unique_ptr<message_class> tcp_server_class::receive_message(){
    std::string message_text;
    int result = tcp_class::receive_message(message_text,256);
    if (result<0) {
        std::cout << "tcp server - error receiving message, result = " << result << std::endl;
        return std::unique_ptr<message_class>(nullptr);
    }

    std::unique_ptr<message_class> test_message(new message_class(message_text,11) );
    return test_message;
}

bool tcp_server_class::send_message(std::unique_ptr<message_class> message){
    int result = tcp_class::send_message(message->get_text());
    return (result > -1);
}
