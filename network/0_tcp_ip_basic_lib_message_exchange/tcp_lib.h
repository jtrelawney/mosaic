// this is from
// https://www.linuxhowtos.org/C_C++/socket.htm

#ifndef _TCP_LIB
#define _TCP_LIB

//#include <string>
#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

class tcp_class{
private:
    int m_port_no, m_socket_fd,m_server_socket_accepted_fd;
    //std::string m_server_ip_address;
    struct hostent *m_server_info;
    struct sockaddr_in m_server_address_info;
    bool m_connection_ok;

    tcp_class(){};
    int close_socket();

public:
    tcp_class(int port_no);
    ~tcp_class();

    struct sockaddr_in prepare_server_address_info(struct hostent *m_server_info);
    struct sockaddr_in prepare_server_address_info();

    int connect_to(struct sockaddr_in m_server_address_info);   // for client to call
    int accept_connection();    // for server to call

    int send_message(std::string message);
    int receive_message(std::string &message);
};

#endif
