#include "message_class.h"

unsigned int message_class::m_message_counter = 0;

message_class::message_class():m_message_data_length(0) {
    // this should be made renentrant
    m_id = m_message_counter;
    m_message_counter++;
    std::cout <<"message class: constructor , id = " << m_id << " , data length = " << m_message_data_length << std::endl;
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

unsigned int message_class::get_id(){
    return m_id;
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

message_class::~message_class(){
    std::cout <<"message class: de-structor " << std::endl;
}
