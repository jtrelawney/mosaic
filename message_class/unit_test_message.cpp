#include <iostream>
#include <test_message_class.h>
#include <memory>


int main(int argc, char *argv[])
{
    //std::unique_ptr<message_class> test_message(new test_message_class("thisisatest") );

    std::cout <<"creating test message 1" << std::endl;
    test_message_class test_message1("thisisatest");
    std::string text = test_message1.get_data();
    std::cout <<"message 1 text = " << text << std::endl;


    // notice the ptr to message_class
    message_class *base_message;
    base_message = &test_message1;

    base_message->prepare_header_buffer();
    char *buffer1 = base_message->get_header_buffer_ptr();

    test_message_class test_message2;
    char *buffer2 = test_message1.get_header_buffer_ptr();

    for (int i=0; i<TCP_HEADER_LENGTH; i++){
        std::cout << int(buffer1[i]) << " , ";
        buffer2[i] = buffer1[i];
    }
    std::cout << std::endl;

    test_message2.set_data(test_message1.get_data());
    std::cout << "test message 2 , text = " << test_message2.get_data() << " id = " << test_message2.get_id() << std::endl;
    



    

/*    bool result = tcp.send_message(std::move(test_message));
    if (result == false) std::cout << "error sending message : " << std::endl;

    std::unique_ptr<message_class> received_message = tcp.receive_message();
    if ( received_message ) {
        std::string received_message_text = received_message->get_text();
        std::cout << "reveived message : " << received_message_text << std::endl;
    } else std::cout << "error receiving message" << std::endl;
*/

    return 0;
}

/*




// it sends a short message to the server and gets one back
// call with the server ip address and the port
// ./tcp.exe 192.168.0.15 8192

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//added those because of warnings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <sstream>


int create_socketfd(){
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) std::cout<<"error creating socket"<<std::end;
	return socket_fd;
}

int get_hostname(std::string server_address, int portno){

	int result = 0;
    printf("gethostname for server address = %s\n",server_addr);

	struct hostent *server;
	server = gethostbyname(server_addr); //gethostbyname(argv[1]);
	//bzero((char *) &m_server_address_info, sizeof(m_server_address_info));

    //fprintf(stderr,"server info\n");
    //char *h_add = server->h_addr;
    //fprintf(stderr,"server address = %s\n",h_add);

    if (server == NULL) {
        result = -1;
    } else {
		m_server_address_info.sin_family = AF_INET;
		bcopy((char *)server->h_addr,
			 (char *)&m_server_address_info.sin_addr.s_addr,
			 server->h_length);
		m_server_address_info.sin_port = htons(portno);
	}

    return result;
}

int get_server_address(struct sockaddr_in *server_data){
	char sadr[] = "192.168.0.15";
	int l = strlen(sadr)+1;
	char *server_addr = (char *) malloc(l*sizeof(char));
	if (server_addr == NULL) exit(0);
	memcpy(server_addr,sadr,l);
	printf("serveraddress = %s   length = %i\n", sadr,l);

    int portno = 8192;
    int result = get_hostname(server_addr, portno);
	std::cout << "hostname resolved" << std::endl;
    return 0;
}

int connect(int sockfd){
    int result;
	std::cout << "connecting" << std::endl;

    struct sockaddr_in server_addr;
    result = get_server_address(&server_addr);



	result = connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if (result < 0) std::cout << "error connecting " << std::endl;
    else std::cout << "connection ready" << std::endl;
	return result;
}

int main(){
    int socket = create_socketfd();
    int status = connect(socket);
    return 0;
}
*/
