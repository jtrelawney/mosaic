#include "tcp_server_class.h"

tcp_server_class::tcp_server_class(int port_number)
    :tcp_class(port_number), newsockfd(-1), m_connection_ok(false){
}

void signal_catcher_server(int signo, siginfo_t *info, void *context)
{
    std::cout <<"\nserver - signalcatcher" << std::endl;
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

int tcp_server_class::non_block_on_off(int on=1){
    if (on !=1 ) {
        std::cout << "server non_block_on_off - on !=1 not implemented yet" << std::endl;
        return -1;
    }

    std::cout <<"\nRT signal - SIGRTMIN + 2 = " << SIGRTMIN + 2 << " for pid = " << getpid() << std::endl;

    struct sigaction act;
    memset(&act, 0, sizeof act);
    act.sa_flags = SA_SIGINFO;//SA_RESTART;                                           //
    struct sigaction old_1;
    memset(&old_1, 0, sizeof old_1);
    act.sa_sigaction = signal_catcher_server;

    if (0 != sigaction(SIGRTMIN + 2, &act, &old_1)) //SIGIO
    {
      perror("sigaction () failed installing SIGUSR1 handler");
      return EXIT_FAILURE;
    }

    return tcp_class::non_block_on_off(on);
}

int tcp_server_class::accept_connection(){
    int result = tcp_class::accept_connection();
    if (result<0) std::cout <<"error accepting connection" << std::endl;
    else m_connection_ok = true;
    return result;
}

int tcp_server_class::send_message(std::string message){
    if (m_connection_ok == false) return -1;
    return tcp_class::send_message(message);
}

int tcp_server_class::receive_message(std::string &message){
    if (m_connection_ok == false) return -1;
    return tcp_class::receive_message(message);
}

tcp_server_class::~tcp_server_class(){

}

/*

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
*/
