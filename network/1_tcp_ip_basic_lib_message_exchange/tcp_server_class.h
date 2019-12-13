// this is from
// https://www.linuxhowtos.org/C_C++/socket.htm

#ifndef _TCP_SERVER
#define _TCP_SERVER

#include <iostream>
#include <tcp_lib.h>

/** TCP server class:\n
    derived from tcp_class
    initializes with a portnumber sets up the server to the point that it is waiting to accept connections\n
    pilot version, later implement the control for :\n
    - header and data reception\n
    - receiving large messages (several packets)\n
    - error handling
    */

class tcp_server_class:public tcp_class{
private:
    tcp_server_class():tcp_class(){};

public:
    tcp_server_class(int port_number);
    ~tcp_server_class();
};

#endif
