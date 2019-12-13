// this is from
// https://www.linuxhowtos.org/C_C++/socket.htm

#ifndef _TCP_SERVER
#define _TCP_SERVER

#include <iostream>
#include <tcp_lib.h>
#include <signal.h>
#include <sys/types.h>

class tcp_server_class:protected tcp_class{
private:
    //std::string m_server_ip_address;
    int newsockfd;
    bool m_connection_ok;
    tcp_server_class():tcp_class(0){};

public:
    tcp_server_class(int port_number);
    ~tcp_server_class();
    int accept_connection();
    int non_block_on_off(int on);

    int send_message(std::string message);
    int receive_message(std::string &message);
};

#endif
