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


/*
int tcp_client_class::send_message(std::string message){
    if (m_connection_ok == false) return -1;
    return tcp_class::send_message(message);
}

int tcp_client_class::receive_message(std::string &message){
    if (m_connection_ok == false) return -1;
    return tcp_class::receive_message(message);
}

*/

tcp_client_class::~tcp_client_class(){
    std::cout << "destructor tcp client" << std::endl;
}
