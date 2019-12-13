// this is from
// https://www.linuxhowtos.org/C_C++/socket.htm

#ifndef _TCP_LIB
#define _TCP_LIB

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/** TCP base class:\n
    initializes with a port number, creates a socket\n
    provides functions for connecting to a server and accepting a connection from a client\n
    provides functions for sending and receiving data\n
    implements very basic error handling\n
    it is a test implementation, hence prints progress info\n
    */

class tcp_class{
protected:
    int m_port_no; ///<the server port the client will connect to
    int m_socket_fd; ///<the socket, created in the constructor for both client and server
    int m_server_socket_accepted_fd; ///<the socket, created on server upon accepting a connection from a client

    tcp_class();
    void close_socket();

public:
    tcp_class(int port_no);
    ~tcp_class();

    int connect_to_server(std::string server_ip_address);
    int accept_client_connection();

    int send_message(std::string message);
    int receive_message(std::string &message);
};

#endif
