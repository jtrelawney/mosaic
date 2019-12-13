// this is from
// https://www.linuxhowtos.org/C_C++/socket.htm

#ifndef _TCP_CLIENT
#define _TCP_CLIENT

#include <iostream>
#include <tcp_lib.h>
#include <signal.h>

class tcp_client_class:protected tcp_class{
private:
    std::string m_server_ip_address;
    bool m_connection_ok;

public:
    tcp_client_class(std::string m_server_ip_address, int port_number);
    ~tcp_client_class();
    int connect_to();
    int non_block_on_off(int on);
    int send_message(std::string message);
    int receive_message(std::string &message);
};

#endif
