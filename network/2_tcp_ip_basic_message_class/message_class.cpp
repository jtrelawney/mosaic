#include "message_class.h"

message_class::message_class(){
    std::cout <<"message class: constructor , do not use" << std::endl;
}

message_class::message_class(std::string text, unsigned int id):message_class(){
    m_message_text = text;
    m_id = id;
    std::cout <<"message class: constructor, text = " <<  m_message_text << std::endl;
}

std::string message_class::get_text(){

    return m_message_text;
    
    int Ns = m_message_text.length()+1;
    int Nid = sizeof(unsigned int);
    int N = Ns + Nid;

    /*
    char buffer[N];
    char *id_p = (char *)&m_id;
    std::strncpy( buffer, id_p, Nid);
    std::strncpy( &buffer[Nid], m_message_text.data(), Ns );
    std::string result(buffer,N);
    */

    char test_buffer[100];
    m_message_text.copy(test_buffer,Ns);
    test_buffer[Ns] = '\0';
    std::string test_result(test_buffer);
    std::cout <<"message class: test = " <<  test_buffer << std::endl;
    return test_result;//m_message_text;
}


message_class::~message_class(){
    std::cout <<"message class: destructor " << std::endl;
}
