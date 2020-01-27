#include "message_class.hpp"

// because message_counter is atomic, this should make the class re-entrant
std::atomic<unsigned int> message_class::m_message_counter(1);

// not to be used
message_class::message_class():
    message_header_class(m_message_counter,message_type_enum::T_undefined_message),
    m_data_ptr(nullptr)
{
    m_message_counter++;
    std::cout <<"message base class: constructor , id = " << m_id << std::endl;
}

message_class::message_class(message_type_enum message_type):
    message_header_class(m_message_counter,message_type),
    m_data_ptr(nullptr)
{
    m_message_counter++;
    std::cout <<"message base class: constructor , id = " << m_id << std::endl;
}

void message_class::prepare_data_buffer(){
}

unsigned int message_class::get_data_length(){
    return m_data_length;
}

char *message_class::get_data_buffer_ptr(){
    return m_data_ptr;
}

unsigned int message_class::get_id(){
    return m_id;
}

message_class::~message_class(){
    std::cout <<"message class: de-structor " << std::endl;
}
