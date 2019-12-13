// this is from
// https://www.linuxhowtos.org/C_C++/socket.htm

#ifndef _TCP_CLIENT
#define _TCP_CLIENT

#include <iostream>
#include <tcp_lib.h>
#include <message_class.h>
#include <memory>

/** TCP client class:\n
    derived from tcp_class
    initializes with a server ip address and connects to the server\n
    pilot version, later implement the control for :\n
    - header and data transfer\n
    - sending large messages (several packets)\n
    - error handling
    */

class tcp_client_class:public tcp_class{

private:
    ///<port number stored in tcp base class
    std::string m_server_ip_address;    ///<ip server address stored here
    tcp_client_class();

public:
    tcp_client_class(std::string m_server_ip_address, int port_number);
    ~tcp_client_class();

    bool send_message(std::unique_ptr<message_class> message);
    std::unique_ptr<message_class> receive_message();
};

#endif
