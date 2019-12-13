#include "tcp_client_class.h"

tcp_client_class::tcp_client_class():tcp_class(), m_server_ip_address("undefined"){
    std::cout << "tcp client: do not call the default constructor, ip = undefined, port = -1" << std::endl;
};

tcp_client_class::tcp_client_class(std::string server_ip_address, int port_number):tcp_class(port_number){
    this->m_server_ip_address = server_ip_address;

    if (m_socket_fd<0) std::cout << "tcp client: constructor call failed" << std::endl;
    else {
        std::cout << "tcp client: constructor call succeeded" << std::endl;
        std::cout << "server ip address = " << m_server_ip_address << std::endl;
        std::cout << "sever port no = " << m_port_no << std::endl;
        std::cout << "client socket fd = " << m_socket_fd << std::endl;
    }

    int result = connect_to_server(m_server_ip_address);
    std::cout << "tcp client: connect to server , result = " << result << std::endl;
}

std::unique_ptr<message_class> tcp_client_class::receive_message(){
    /*
    std::string message_text;
    int result = tcp_class::receive_message(message_text);
    */
    char buffer[256] = {0};
    int N_received = tcp_class::receive_message(buffer,256);
    std::string message_text = std::string(buffer, N_received);

    if (N_received<0) {
        std::cout << "tcp server - error receiving message, N_received = " << N_received << std::endl;
        return std::unique_ptr<message_class>(nullptr);
    }

    std::unique_ptr<message_class> test_message(new message_class(message_text,7) );
    return test_message;
}

bool tcp_client_class::send_message(std::unique_ptr<message_class> message){
    int result = tcp_class::send_message(message->get_text());
    return (result > -1);
}

tcp_client_class::~tcp_client_class(){
    std::cout << "destructor tcp client" << std::endl;
}
