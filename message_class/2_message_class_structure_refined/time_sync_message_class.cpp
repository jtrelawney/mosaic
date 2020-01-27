#include <time_sync_message_class.hpp>

time_sync_message_class::time_sync_message_class():
    m_1_event_origin(0),
    m_2_tcp_send_origin(0),
    m_3_tcp_receipt_target(0),
    m_4_tcp_send_target(0),
    m_5_tcp_receipt_origin(0),
    m_6_tcp_send_origin(0),
    m_7_tcp_receipt_target(0)
{
    m_sensor_time_stamp = get_time_stamp_ns();
    // set the message members for access through the base class
    m_data_ptr = (char *)m_data_buffer;
    m_data_length = m_buffer_length;
    //std::cout <<"m_data_length = " << m_data_length << std::endl;
    //std::cout <<"m_data_ptr = " << (void *)m_data_ptr << std::endl;
}

void time_sync_message_class::print_data(){
    std::cout <<"\n\ntime_sync_data:" << std::endl;
    std::cout <<"m_1_event_origin = " << m_1_event_origin << std::endl;
    std::cout <<"m_2_tcp_send_origin = " << m_2_tcp_send_origin << std::endl;
    unsigned int d = m_2_tcp_send_origin-m_1_event_origin;
    if (d>0) std::cout << "delta = " << d << std::endl;
    std::cout <<"m_3_tcp_receipt_target = " << m_3_tcp_receipt_target << std::endl;
    d = m_3_tcp_receipt_target - m_2_tcp_send_origin;
    if (d>0) std::cout << "delta = " << d << std::endl;
    std::cout <<"m_4_tcp_send_target = " << m_4_tcp_send_target << std::endl;
    d = m_4_tcp_send_target - m_3_tcp_receipt_target;
    if (d>0) std::cout << "delta = " << d << std::endl;
    std::cout <<"m_5_tcp_receipt_origin = " << m_5_tcp_receipt_origin << std::endl;
    d = m_5_tcp_receipt_origin - m_4_tcp_send_target;
    if (d>0) std::cout << "delta = " << d << std::endl;
    std::cout <<"m_6_tcp_send_origin = " << m_6_tcp_send_origin << std::endl;
    d = m_6_tcp_send_origin - m_5_tcp_receipt_origin;
    if (d>0) std::cout << "delta = " << d << std::endl;
    std::cout <<"m_7_tcp_receipt_target = " << m_7_tcp_receipt_target << std::endl;
    d = m_7_tcp_receipt_target - m_6_tcp_send_origin;
    if (d>0) std::cout << "delta = " << d << std::endl;
}

void time_sync_message_class::prepare_data_buffer(){
    std::cout <<"time sync class - prepare data buffer" << std::endl;
    def_time_format *pointer = m_data_buffer;
    *pointer=m_1_event_origin;
    pointer++;
    *pointer=m_2_tcp_send_origin;
    pointer++;
    *pointer=m_3_tcp_receipt_target;
    pointer++;
    *pointer=m_4_tcp_send_target;
    pointer++;
    *pointer=m_5_tcp_receipt_origin;
    pointer++;
    *pointer=m_6_tcp_send_origin;
    pointer++;
    *pointer=m_7_tcp_receipt_target;
}

// the data has been copied into the buffer and this popluates the members with the data
void time_sync_message_class::extract_data_buffer(){
    def_time_format *pointer = m_data_buffer;
    m_1_event_origin = *pointer;
    pointer++;
    m_2_tcp_send_origin = *pointer;
    pointer++;
    m_3_tcp_receipt_target = *pointer;
    pointer++;
    m_4_tcp_send_target = *pointer;
    pointer++;
    m_5_tcp_receipt_origin = *pointer;
    pointer++;
    m_6_tcp_send_origin = *pointer;
    pointer++;
    m_7_tcp_receipt_target = *pointer;
}

time_sync_message_class::~time_sync_message_class(){
}
