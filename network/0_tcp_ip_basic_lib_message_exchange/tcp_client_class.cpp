#include "tcp_client_class.h"

tcp_client_class::tcp_client_class(std::string server_ip_address, int port_number)
    :tcp_class(port_number), m_server_ip_address(server_ip_address), m_connection_ok(false){

    // The gethostbyname() function returns a structure of type hostent for the given host name
    // struct hostent *gethostbyname(const char *name);

    //struct hostent *m_server_info;
    //struct sockaddr m_server_address_info;
    //std::cout <<"get host info" << std::endl;
    struct hostent *m_server_info = gethostbyname(m_server_ip_address.c_str());
    if (m_server_info == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    struct sockaddr_in m_server_address_info = prepare_server_address_info(m_server_info);
    int result = tcp_class::connect_to(m_server_address_info);
    if (result==0) m_connection_ok=true;
}



int tcp_client_class::send_message(std::string message){
    if (m_connection_ok == false) return -1;
    return tcp_class::send_message(message);
}

int tcp_client_class::receive_message(std::string &message){
    if (m_connection_ok == false) return -1;
    return tcp_class::receive_message(message);
}

tcp_client_class::~tcp_client_class(){
    std::cout << "destructor tcp client" << std::endl;
}
