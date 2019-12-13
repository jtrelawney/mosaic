#include "message_class.h"

unsigned int message_class::m_message_counter = 0;

message_class::message_class():m_message_text(""), m_message_data_length(0) {
    // this should be made renentrant
    m_id = m_message_counter;
    m_message_counter++;
    std::cout <<"message class: constructor , id = " << m_id << " , data length = " << m_message_data_length << std::endl;
    print_buffer_content();
}

/*
message_class::message_class(unsigned int data_length):message_class() {
    //std::cout <<"message class: constructor , id = " << m_id << " , data length = " << m_message_data_length << std::endl;
    m_message_data_length = data_length;
}
*/

// creates a message from given text
message_class::message_class(std::string text):message_class(){
    m_message_text = text;
    int N = text.length();
    m_message_data_length = N;

    std::cout <<"message class: constructor , id = " << m_id << " , data length = " << m_message_data_length << std::endl;
    std::cout <<"message = " << text << std::endl;
    print_buffer_content();
}

unsigned int message_class::get_message_data_length(){
    return m_message_data_length;
}

void message_class::print_buffer_content(){
    std::cout << "Buffercontent = ";
    for (int i = 0; i< TCP_HEADER_LENGTH; i++) std::cout << (int)m_header_buffer[i] <<",";
    std::cout << std::endl;
}

// from the buffer create the meta data
bool message_class::extract_header_buffer(){
    std::cout <<"message class : extract meta data from headerbuffer" << std::endl;

    unsigned int *uiptr = &m_id;
    int N = sizeof(m_id), pos = 0;
    memcpy((char *)uiptr, &m_header_buffer[pos], N);
    pos+=N;
    std::cout <<"message id = " << m_id << std::endl;

    uiptr = &m_message_data_length;
    N = sizeof(m_message_data_length);
    memcpy((char *)uiptr, &m_header_buffer[pos], sizeof(m_message_data_length));
    pos+=N;
    std::cout <<"data length = " << m_message_data_length << std::endl;

    return true;
}

// from the meta data fill the buffer
// the constructor prepares the buffer
bool message_class::prepare_header_buffer(){
    std::cout <<"message class : prepare header with metadata" << std::endl;
    unsigned int *uiptr = &m_id;
    int N = sizeof(m_id), pos = 0;
    memcpy(&m_header_buffer[pos], (char *)uiptr, N);
    pos+=N;

    uiptr = &m_message_data_length;
    N = sizeof(m_message_data_length);
    memcpy(&m_header_buffer[pos], (char *)uiptr, sizeof(m_message_data_length));
    pos+=N;

    return true;
}

// gets the pointer, used for tcp to fill the header buffer
char* message_class::get_header_buffer_ptr(){
    print_buffer_content();
    return m_header_buffer;
}

void message_class::set_text(std::string text){
    m_message_text = text;
}

std::string message_class::get_text(){

    return m_message_text;

    unsigned int *uiptr = &m_id;
    int N = sizeof(m_id), pos = 0;
    memcpy(&m_header_buffer[pos], (char *)uiptr, N);
    pos+=N;

    uiptr = &m_message_data_length;
    N = sizeof(m_message_data_length);
    memcpy(&m_header_buffer[pos], (char *)uiptr, sizeof(m_message_data_length));
    pos+=N;

    std::cout << "Buffercontent = ";
    for (int i = 0; i< TCP_HEADER_LENGTH; i++) std::cout << (int)m_header_buffer[i] <<",";
    std::cout << std::endl;

    return std::string(m_header_buffer,TCP_HEADER_LENGTH);

    //int Ns = m_message_text.length()+1;
    //int Nid = sizeof(unsigned int);
    //int N = Ns + Nid;

    /*
    char buffer[N];
    char *id_p = (char *)&m_id;
    std::strncpy( buffer, id_p, Nid);
    std::strncpy( &buffer[Nid], m_message_text.data(), Ns );
    std::string result(buffer,N);
    */
/*
    char test_buffer[100];
    m_message_text.copy(test_buffer,Ns);
    test_buffer[Ns] = '\0';
    std::string test_result(test_buffer);
    std::cout <<"message class: test = " <<  test_buffer << std::endl;
    return test_result;//m_message_text;
    */
}


message_class::~message_class(){
    std::cout <<"message class: destructor " << std::endl;
}
