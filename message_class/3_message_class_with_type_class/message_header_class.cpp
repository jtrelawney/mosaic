#include "message_header_class.hpp"

/*
message_header_class::message_header_class(unsigned int id):
    m_message_type(message_type_enum::T_undefined_message),
    m_id(id),
    m_data_length(0),
    m_sensor_time_stamp(0)
{
}
*/

message_header_class::message_header_class(unsigned int id, message_type_enum message_type):
    m_message_type(message_type),
    m_id(id),
    m_data_length(0),
    m_sensor_time_stamp(0)
{
}

message_header_class::message_header_class():message_header_class(0,message_type_enum::T_undefined_message)
{
}

message_header_class::~message_header_class(){
}

void message_header_class::print_header_data(){
    std::cout << "\n\nHeader data:" << std::endl;
    std::cout << "m_message_type = " << m_message_type << std::endl;
    std::cout << "m_id = " << m_id << std::endl;
    std::cout << "m_data_length = " << m_data_length << std::endl;
    std::cout << "m_sensor_time_stamp = " << m_sensor_time_stamp << std::endl;
}

void message_header_class::print_header_buffer_content(){
    std::cout << "Header - Buffercontent = ";
    unsigned int buffer_length = get_header_length();
    char *buffer_ptr = get_header_buffer_ptr();
    for (unsigned int i = 0; i<buffer_length; i++) {
        std::cout << (int)(*buffer_ptr) <<",";
        buffer_ptr++;
    }
    std::cout << std::endl;
}

unsigned int message_header_class::get_header_length(){
    return TCP_HEADER_LENGTH;
}

char* message_header_class::get_header_buffer_ptr(){
    return m_header_buffer;
}

void message_header_class::populate_buffer_from_member_data(){
    int pos = 0; // starting at beginning of buffer fill in the member data and increase pos for each step
    int count = 0; // keep count of bytes written to buffer

    //std::cout <<"message class : prepare header with metadata" << std::endl;
    unsigned int *ui_ptr = &m_id;
    int N = sizeof(m_id);
    std::memcpy(&m_header_buffer[pos], (char *)ui_ptr, N);
    pos+=N, count+=N;

    ui_ptr = &m_data_length;
    N = sizeof(m_data_length);
    memcpy(&m_header_buffer[pos], (char *)ui_ptr, N);
    pos+=N, count+=N;

    def_time_format *tf_ptr = &m_sensor_time_stamp;
    N = sizeof(m_sensor_time_stamp);
    memcpy(&m_header_buffer[pos], (char *)tf_ptr, N);
    pos+=N, count+=N;

    message_type_enum *mt_ptr = &m_message_type;
    N = sizeof(message_type_enum);
    memcpy(&m_header_buffer[pos], (char *)mt_ptr, N);
    pos+=N, count+=N;

    // the number of bytes written must fit in buffer size
    assert (count<=TCP_HEADER_LENGTH);
}

void message_header_class::populate_member_data_from_buffer(){
    //std::cout <<"message class : extract meta data from headerbuffer" << std::endl;
    int pos = 0;    // index in buffer, increases by the item size
    int count = 0;  // running counter to check on appropriate index

    unsigned int *ui_ptr = &m_id;
    int N = sizeof(m_id);
    count+=N;
    assert (count<=TCP_HEADER_LENGTH);
    memcpy((char *)ui_ptr, &m_header_buffer[pos], N);
    pos+=N;

    ui_ptr = &m_data_length;
    N = sizeof(m_data_length);
    count+=N;
    assert (count<=TCP_HEADER_LENGTH);
    memcpy((char *)ui_ptr, &m_header_buffer[pos], N);
    pos+=N;

    def_time_format *tf_ptr = &m_sensor_time_stamp;
    N = sizeof(m_sensor_time_stamp);
    count+=N;
    assert (count<=TCP_HEADER_LENGTH);
    memcpy((char *)tf_ptr, &m_header_buffer[pos], N);
    pos+=N;

    message_type_enum *mt_ptr = &m_message_type;
    N = sizeof(m_message_type);
    count+=N;
    assert (count<=TCP_HEADER_LENGTH);
    memcpy((char *)mt_ptr, &m_header_buffer[pos], N);
    pos+=N;
}
