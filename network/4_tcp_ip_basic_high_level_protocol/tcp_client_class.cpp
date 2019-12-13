#include "tcp_client_class.h"

tcp_client_class::tcp_client_class():tcp_class(), m_server_ip_address("undefined"){
    std::cout << "tcp client: do not call the default constructor, ip = undefined, port = -1" << std::endl;
};

tcp_client_class::tcp_client_class(std::string server_ip_address, int port_number):tcp_class(port_number){
    this->m_server_ip_address = server_ip_address;

    if (m_socket_fd<0) std::cout << "tcp client: constructor call failed" << std::endl;
    else {
        std::cout << "tcp client: constructor call succeeded" << std::endl;
        std::cout << "server ip address = " << m_server_ip_address << std::endl;
        std::cout << "sever port no = " << m_port_no << std::endl;
        std::cout << "client socket fd = " << m_socket_fd << std::endl;
    }

    int result = connect_to_server(m_server_ip_address);
    std::cout << "tcp client: connect to server , result = " << result << std::endl;
}

std::unique_ptr<message_class> tcp_client_class::receive_message(){

    message_class *message = new message_class();
    char *header_buffer_ptr = message->get_header_buffer_ptr();
    int header_result = tcp_class::receive_message(header_buffer_ptr, TCP_HEADER_LENGTH);

    if (header_result<0) {
        std::cout << "tcp client - error receiving message, result = " << header_result << std::endl;
        return std::unique_ptr<message_class>(nullptr);
    }
    else if (header_result==0) {
        std::cout << "tcp client - received N = " << header_result << std::endl;
        std::cout << "peer has disconnected" << std::endl;
        return std::unique_ptr<message_class>(nullptr);
    }
    else {
        std::cout << "tcp client - received N = " << header_result << std::endl;
        message->extract_header_buffer();
    }

    unsigned int message_data_length = (*message).get_message_data_length();
    std::cout << "tcp client - going to read data of length = " << message_data_length << std::endl;
    char text_buffer[message_data_length];
    int data_result = tcp_class::receive_message(text_buffer, message_data_length);

    if (data_result<0) {
        std::cout << "tcp client - error receiving message, result = " << data_result << std::endl;
        return std::unique_ptr<message_class>(nullptr);
    }
    else {
        std::cout << "tcp client - received N = " << data_result << std::endl;
        std::string message_text(text_buffer,message_data_length);
        message->set_text(message_text);
    }

    std::unique_ptr<message_class> new_message(message);
    return new_message;
}

/// this sends the complete message, first the header and later the data
bool tcp_client_class::send_message(std::unique_ptr<message_class> message_ptr){
    message_class *message = message_ptr.get();
    message->prepare_header_buffer();
    char *header_buffer_ptr = message->get_header_buffer_ptr();
    int header_result = tcp_class::send_message(header_buffer_ptr, TCP_HEADER_LENGTH);

    std::string text = message->get_text();
    std::cout << "tcp server - now sending text = " << text << std::endl;
    int data_result = tcp_class::send_message(text);

    bool result = !( (header_result<0) | (data_result<0) );
    std::cout << "tcp client - send result = " << result << std::endl;
    return result;
}

tcp_client_class::~tcp_client_class(){
    std::cout << "destructor tcp client" << std::endl;
}
