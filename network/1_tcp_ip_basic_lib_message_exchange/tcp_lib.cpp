#include "tcp_lib.h"

tcp_class::tcp_class():m_port_no(-1), m_socket_fd(-1),m_server_socket_accepted_fd(-1){
    m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket_fd>-1) {
        std::cout <<"tcp base class: constructor created socket = " << m_socket_fd << std::endl;
    } else {
        std::cout << "tcp base class: constructor call failed" << std::endl;
        error("ERROR opening socket");
    }
}

tcp_class::tcp_class(int port_no):tcp_class(){
    this->m_port_no = port_no;
    if (m_socket_fd<0) std::cout << "tcp base class: constructor call failed" << std::endl;
    //else std::cout <<"tcp base class: constructor created socket = " << m_socket_fd << std::endl;
}

void tcp_class::close_socket(){

    if (m_server_socket_accepted_fd>-1) {
            std::cout << "tcp base - closing socket to client" << std::endl;
            close(m_server_socket_accepted_fd);
        }

    if (m_socket_fd<0) {
        std::cout << "tcp base - close() : no socket to close" << std::endl;
    } else {
        std::cout << "tcp base - closing socket" << std::endl;
        close(m_socket_fd);
    }
}

tcp_class::~tcp_class(){
    std::cout << "destructor tcp base" << std::endl;
    close_socket();
}

int tcp_class::connect_to_server(std::string server_ip_address){
    std::cout << "tcb base class - connect to server " << server_ip_address << " , port = " << m_port_no << " , fd = " << m_socket_fd << std::endl;

    // The gethostbyname() function returns a structure of type hostent for the given host name
    // manpage struct hostent *gethostbyname(const char *name);
    struct hostent *server_info = gethostbyname(server_ip_address.c_str());
    if (server_info == NULL) {
        std::cout << "tcb base class - failed to create server_info " << server_ip_address.c_str() << std::endl;
        return -1;
    } else std::cout << "tcb base class - server_info created " << std::endl;

    struct sockaddr_in server_address_info = {0};
    server_address_info.sin_family = AF_INET;
    // set internet address , sin_addr
    // it only has one entry: s_addr, which is the address in network byte order
    //uint32_t *test = (uint32_t *)server_info->h_addr;
    //server_address_info.sin_addr.s_addr = *test;
    server_address_info.sin_port = htons(m_port_no);
    // old stuff

    bcopy((char *)server_info->h_addr,
         (char *)&server_address_info.sin_addr.s_addr,
         server_info->h_length);

    int result = connect(m_socket_fd,(struct sockaddr *) &server_address_info,sizeof(server_address_info));
    if ( result < 0) {
        std::string error_message = "ERROR (" + std::to_string(result) + ") connecting";
        error(error_message.c_str());
    }
    return result;
}

int tcp_class::accept_client_connection(){

    // this avoids the "socket already in use error when the server is started at a time the port has not been released yet"
    int set_flag = 1;
    setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &set_flag, sizeof(set_flag));

    std::cout <<"tcp server : bind()" << std::endl;
    struct sockaddr_in serv_addr = {0};
    //bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(m_port_no);
    if (bind(m_socket_fd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
             error("ERROR on binding");
    std::cout <<"tcp server : listen()" << std::endl;
    listen(m_socket_fd,5);

    std::cout <<"tcp server : accept()" << std::endl;
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    m_server_socket_accepted_fd = accept(m_socket_fd,
                (struct sockaddr *) &cli_addr,
                &clilen);
    if (m_server_socket_accepted_fd < 0)
         error("ERROR on accept");
    std::cout <<"connection accepted , fd = " << m_server_socket_accepted_fd << std::endl;
    return m_server_socket_accepted_fd;
 }

 int tcp_class::receive_message(std::string &message){
     char buffer[256] = {0};
     //bzero(buffer,256);
     int N_received = -1;

     if (m_server_socket_accepted_fd<0){
         std::cout <<"tcp base : client reading message on server connection socket" << std::endl;
         N_received = read(m_socket_fd,buffer,256);
     }
     else {
         std::cout <<"tcp base : server reading message on client connection socket" << std::endl;
         N_received = read(m_server_socket_accepted_fd,buffer,256);
     }

     if (N_received < 0)
         error("ERROR reading from socket");

     message = std::string(buffer, N_received);
     return N_received;
 }

 int tcp_class::send_message(std::string message){
     int N = message.length();
     char buffer[N];
     strcpy(buffer, message.c_str());
     int N_sent=-1;

     if (m_server_socket_accepted_fd<0){
         std::cout <<"tcp base : client sending message on server connection socket" << std::endl;
         N_sent = write(m_socket_fd,buffer,strlen(buffer));
     }
     else {
         std::cout <<"tcp base : server sending message on client connection socket" << std::endl;
         N_sent = write(m_server_socket_accepted_fd,buffer,strlen(buffer));
     }

     if (N_sent < 0)
         error("ERROR writing to socket");
     return N_sent;
 }
