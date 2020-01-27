#include "message_class.hpp"

std::atomic<unsigned int> message_class::m_message_counter(1);

// because message_counter is atomic, this should be re-entrant
message_class::message_class():
    message_header_class(m_message_counter),
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
