#include <iostream>
#include <tcp_server_class.h>

int main(int argc, char *argv[])
{
    int result = -1;

    tcp_server_class tcp(8192);
    result = tcp.accept_connection();
    if (result<0) std::cout << "error accepting connection" << std::endl;

    std::string received_message;
    result = tcp.receive_message(received_message);
    if (result<0) std::cout << "error receiving message" << std::endl;
    else std::cout << "reveived message : " << received_message << std::endl;

    std::string response = "I got the message !";
    result = tcp.send_message(response);
    if (result<0) std::cout << "error sending response" << std::endl;

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
