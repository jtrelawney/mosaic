#include <iostream>
#include <tcp_server_class.h>
#include <message_class.h>
#include <memory>

tcp_server_class tcp(8192);

bool send_test_message(std::string message){
    std::string full_message = "response - " + message;
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
    int server_ready = tcp.accept_client_connection();
    if (server_ready<0) {
        std::cout << "error accepting connection" << std::endl;
        return 0;
    }

    unsigned int message_counter = 0;
    bool messaging_continues = true;
    while (messaging_continues){
        std::cout << std::endl << std::endl;
        send_test_message(std::to_string(message_counter));
        messaging_continues = receive_test_message();
        message_counter++;
    }

    return 0;
}


/* A simple server in the internet domain using TCP
   The port number is passed as an argument */

   /*
#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = 8192;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
     return 0;
}
*/
