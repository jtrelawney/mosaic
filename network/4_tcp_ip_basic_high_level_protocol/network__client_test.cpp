#include <iostream>
#include <tcp_client_class.h>
#include <message_class.h>
#include <memory>

tcp_client_class tcp("192.168.0.15",8192);

bool send_test_message(std::string message){
    std::string full_message = "test - " + message;
    std::unique_ptr<message_class> test_message(new message_class(full_message) );
    bool result = tcp.send_message(std::move(test_message));
    if (result == false) std::cout << "error sending message : " << std::endl;
    return result;
}

bool receive_test_message(){
    std::unique_ptr<message_class> received_message = tcp.receive_message();
    bool result = false;
    if ( received_message ) {
        std::string received_message_text = received_message->get_text();
        std::cout << "reveived message : " << received_message_text << std::endl;
        result = true;
    } else std::cout << "error receiving message" << std::endl;
    return result;
}

int main(int argc, char *argv[])
{

    for (int i=0;i<2;i++){
        send_test_message(std::to_string(i));
        receive_test_message();
    }

    return 0;
}
