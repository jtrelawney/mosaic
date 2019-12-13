#include "tcp_server_class.h"

tcp_server_class::tcp_server_class(int port_number):tcp_class(port_number) {
    if (m_socket_fd<0) std::cout << "tcp server: constructor call failed" << std::endl;
    else {
        std::cout << "tcp server: constructor call succeeded" << std::endl;
        std::cout << "sever port no = " << m_port_no << std::endl;
        std::cout << "server socket fd = " << m_socket_fd << std::endl;
    }
}

tcp_server_class::~tcp_server_class(){
    std::cout << "destructor tcp server" << std::endl;
}

std::unique_ptr<message_class> tcp_server_class::receive_message(){

    message_class *message = new message_class();
    char *header_buffer_ptr = message->get_header_buffer_ptr();
    int header_result = tcp_class::receive_message(header_buffer_ptr, TCP_HEADER_LENGTH);

    if (header_result<0) {
        std::cout << "tcp server - error receiving message, result = " << header_result << std::endl;
        return std::unique_ptr<message_class>(nullptr);
    }
    else if (header_result==0) {
        std::cout << "tcp client - received N = " << header_result << std::endl;
        std::cerr << "peer has disconnected" << std::endl;
        return std::unique_ptr<message_class>(nullptr);
    }
    else {
        std::cout << "tcp server - received N = " << header_result << std::endl;
        message->extract_header_buffer();
    }

    unsigned int message_data_length = (*message).get_message_data_length();
    std::cout << "tcp server - going to read data of length = " << message_data_length << std::endl;
    char text_buffer[message_data_length];
    int data_result = tcp_class::receive_message(text_buffer, message_data_length);

    if (data_result<0) {
        std::cout << "tcp server - error receiving message, result = " << data_result << std::endl;
        return std::unique_ptr<message_class>(nullptr);
    }
    else {
        std::cout << "tcp server - received N = " << data_result << std::endl;
        std::string message_text(text_buffer,message_data_length);
        message->set_text(message_text);
    }

    std::unique_ptr<message_class> new_message(message);
    return new_message;
}

bool tcp_server_class::send_message(std::unique_ptr<message_class> message_ptr){
    message_class *message = message_ptr.get();
    message->prepare_header_buffer();
    char *header_buffer_ptr = message->get_header_buffer_ptr();
    int header_result = tcp_class::send_message(header_buffer_ptr, TCP_HEADER_LENGTH);

    std::string text = message->get_text();
    std::cout << "tcp server - now sending text = " << text << std::endl;
    int data_result = tcp_class::send_message(text);

    bool result = !( (header_result<0) | (data_result<0) );
    std::cout << "tcp server - send result = " << result << std::endl;
    return result;


    //int result = tcp_class::send_message(message->get_text());
    //return (result > -1);
}
