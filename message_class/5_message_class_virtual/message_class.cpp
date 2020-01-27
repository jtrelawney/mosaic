#include "message_class.hpp"


message_class::message_class(message_type_enum mtype, unsigned int mid):
    m_message_type(mtype),
    m_id(mid),
    m_sensor_time_stamp(16)
{
    std::cout << "message_class constructor" << std::endl;
    //print_meta_data();
}

// only set the buffer, need to call populate_member_data_from_buffer to complete the initialization
message_class::message_class(header_buffer_class &buffer){
    std::cout << "message class constructor with buffer" << std::endl;
    set_buffer(buffer);
}

message_class::~message_class(){
    std::cout << "message_class de - structor" <<std::endl;
}

buffer_info message_class::get_header_buffer_info(){
    buffer_info bi;
    bi.set_info(get_buffer_length(),get_buffer_ptr());
    return bi;
}

void message_class::print_meta_data(){
    std::cout << "\n\nHeader data:" << std::endl;
    std::cout << "m_message_type = " << m_message_type << std::endl;
    std::cout << "m_id = " << m_id << std::endl;
    std::cout << "m_sensor_time_stamp = " << m_sensor_time_stamp << std::endl;
}

buffer_info message_class::get_data_buffer_info(){
    buffer_info bi;
    bi.set_info(0,nullptr);
    return bi;
}

message_type_enum message_class::get_message_type(){
    return m_message_type;
}

// transfers information from message class members into the header buffer
unsigned int message_class::populate_header_buffer(){
    unsigned int N = 0;
    N+=set_buffer_data(m_message_type, N);
    std::cout <<"N = " << N << std::endl;
    N+=set_buffer_data(m_id, N);
    std::cout <<"N = " << N << std::endl;
    N+=set_buffer_data(m_sensor_time_stamp, N);
    std::cout <<"N = " << N << std::endl;
    //print_header_buffer_content();
    return N;
}

// this is declared virtual
// if the derived class has more members to be transfered it can overwrite this function and
// then call the message class populate_header_buffer() first to transfer the base class members
// and after that transfer its derived members
unsigned int message_class::populate_header_buffer_from_member_data(){
    return populate_header_buffer();
}

// this is declared virtual
// if the derived class has more members to be transfered it can overwrite this function and
// then call the message class populate_member_data() first to transfer the base class members
// and after that transfer its derived members
unsigned int message_class::populate_member_data_from_buffer(header_buffer_class &buffer){
    return populate_member_data();
}

// transfers information from heder buffer into the message class members
unsigned int message_class::populate_member_data(){
    unsigned int N = 0;
    N+=get_buffer_data(m_message_type, N);
    std::cout <<"N = " << N << std::endl;
    N+=get_buffer_data(m_id, N);
    std::cout <<"N = " << N << std::endl;
    N+=get_buffer_data(m_sensor_time_stamp, N);
    std::cout <<"N = " << N << std::endl;
    //print_header_buffer_content();
    return N;
}
