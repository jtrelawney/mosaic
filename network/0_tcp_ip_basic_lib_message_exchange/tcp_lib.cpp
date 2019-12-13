#include "tcp_lib.h"

tcp_class::tcp_class(int port_number):m_port_no(port_number),m_socket_fd(-1),m_server_socket_accepted_fd(-1),m_server_info(NULL),m_connection_ok(false){
    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket_fd < 0) error("ERROR opening socket");
}

struct sockaddr_in tcp_class::prepare_server_address_info(struct hostent *m_server_info){
    struct sockaddr_in m_server_address_info;
    bzero((char *) &m_server_address_info, sizeof(m_server_address_info));
    // set address family = sin_family
    m_server_address_info.sin_family = AF_INET;
    // set internet address , sin_addr
    // it only has one entry: s_addr, which is the address in network byte order
    bcopy((char *)m_server_info->h_addr,
         (char *)&m_server_address_info.sin_addr.s_addr,
         m_server_info->h_length);
    m_server_address_info.sin_port = htons(m_port_no);
    return m_server_address_info;
}
struct sockaddr_in tcp_class::prepare_server_address_info(){
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(m_port_no);
    return serv_addr;
}

int tcp_class::accept_connection(){
    //struct sockaddr_in serv_addr = prepare_server_address_info();
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(m_port_no);
    std::cout <<"trying bind" << std::endl;
    // this avoids the "socket already in use error when the server is started at a time the port has not been released yet"
    int set_flag = 1;
    setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &set_flag, sizeof(set_flag));

    if (bind(m_socket_fd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
             error("ERROR on binding");
    std::cout <<"trying listen" << std::endl;
    listen(m_socket_fd,5);
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);
    std::cout <<"trying accept" << std::endl;
    m_server_socket_accepted_fd = accept(m_socket_fd,
                (struct sockaddr *) &cli_addr,
                &clilen);
    if (m_server_socket_accepted_fd < 0)
         error("ERROR on accept");
    std::cout <<"connection accepted , fd = " << m_server_socket_accepted_fd << std::endl;
    return m_server_socket_accepted_fd;
 }

int tcp_class::connect_to(struct sockaddr_in m_server_address_info){
    int result = connect(m_socket_fd,(struct sockaddr *) &m_server_address_info,sizeof(m_server_address_info));
    if ( result < 0)
        error("ERROR connecting");
    else m_connection_ok = true;
    return result;
}

int tcp_class::send_message(std::string message){
    int N = message.length();
    char buffer[N];
    strcpy(buffer, message.c_str());
    int N_sent=-1;
    if (m_server_socket_accepted_fd<0){
        std::cout <<"sending client message" << std::endl;
        N_sent = write(m_socket_fd,buffer,strlen(buffer));
    }
    else {
        std::cout <<"sending server message" << std::endl;
        N_sent = write(m_server_socket_accepted_fd,buffer,strlen(buffer));
    }
    if (N_sent < 0)
        error("ERROR writing to socket");
    return N_sent;
}

int tcp_class::receive_message(std::string &message){
    char buffer[256];
    bzero(buffer,256);

    int N_received;
    if (m_server_socket_accepted_fd<0){
        std::cout <<"reading client message" << std::endl;
        N_received = read(m_socket_fd,buffer,256);
    }
    else {
        std::cout <<"reading server message" << std::endl;
        N_received = read(m_server_socket_accepted_fd,buffer,256);
    }

    if (N_received < 0)
        error("ERROR reading from socket");

    message = std::string(buffer, N_received);
    return N_received;
}

int tcp_class::close_socket(){
    if (m_server_socket_accepted_fd>-1) {
        std::cout << "tcp base - closing connection socket" << std::endl;
        close(m_server_socket_accepted_fd);
    }
    if (m_socket_fd>-1) {
        std::cout << "tcp base - closing server socket" << std::endl;
        close(m_socket_fd);
    }
    return -100;
}

tcp_class::~tcp_class(){
    std::cout << "destructor tcp base" << std::endl;
    close_socket();
}
