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

void signal_catcher(int signo, siginfo_t *info, void *context)
{
    std::cout <<"\nsignalcatcher" << std::endl;
    std::cout << "signo " << info->si_signo << std::endl;
    std::cout <<"errno " << info->si_errno << std::endl;
    std::cout <<"fd " << info->si_fd << std::endl;
    std::cout <<"band " << info->si_band << std::endl;
    std::cout <<"int " << info->si_int << std::endl;
    std::cout <<"sigval " << info->si_value.sival_int << std::endl;

    std::cout <<"code " << info->si_code << std::endl;
    switch(info->si_code) {
        case POLL_OUT: std::cout << "POLL_OUT = Output buffers available." << POLL_OUT << std::endl; break;
        case POLL_IN: std::cout << "POLL_IN = Data input available." << POLL_IN << std::endl; break;
        case POLL_MSG: std::cout << "Input message available." << POLL_OUT << std::endl; break;
        case POLL_ERR: std::cout << "I/O error." << POLL_IN << std::endl; break;
        case POLL_PRI: std::cout << "High priority input available." << POLL_OUT << std::endl; break;
        case POLL_HUP: std::cout << "Device disconnected." << POLL_IN << std::endl; break;
        default:
            std::cout << POLL_OUT << " " << POLL_IN << " " << POLL_MSG<< " " << POLL_ERR<< " " << POLL_PRI<< " " << POLL_HUP << std::endl;
    }
}

int tcp_client_class::non_block_on_off(int on=1){
    if (on !=1 ) {
        std::cout << "server non_block_on_off - on !=1 not implemented yet" << std::endl;
        return -1;
    }

    std::cout <<"\nRT signal - SIGRTMIN + 1 = " << SIGRTMIN + 1 << " for pid = " << getpid() << std::endl;

    struct sigaction act;
    memset(&act, 0, sizeof act);
    act.sa_flags = SA_SIGINFO;//SA_RESTART;                                           //
    struct sigaction old_1;
    memset(&old_1, 0, sizeof old_1);
    act.sa_sigaction = signal_catcher;

    if (0 != sigaction(SIGRTMIN + 1, &act, &old_1)) //SIGIO
    {
      perror("sigaction () failed installing SIGUSR1 handler");
      return EXIT_FAILURE;
    }

    return tcp_class::non_block_on_off(on);
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
