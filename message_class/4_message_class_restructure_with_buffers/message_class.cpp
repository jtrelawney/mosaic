#include "message_class.hpp"

message_class::message_class():
    m_message_type(message_type_enum::T_undefined_message),
    m_data_ptr(nullptr),
    m_id(0),
    m_data_length(0),
    m_sensor_time_stamp(0)
{
}

message_class::message_class(unsigned int id, other_data_class data):
    m_message_type(message_type_enum::T_other_message),
    m_other_data(data),
    m_data_ptr(nullptr),
    m_id(id),
    m_data_length(0),
    m_sensor_time_stamp(0)
{
    // setting those in the initializer list above doesn't work because the data parameter is temporary
    m_data_ptr = m_other_data.get_data_ptr();
    m_data_length = m_other_data.get_data_length();
}

message_class::message_class(header_buffer_class &buffer){
    populate_member_data_from_buffer(buffer);
    std::cout << "creating : " << m_message_type << " , l = " << m_data_length << std::endl;
    switch(m_message_type){
        case message_type_enum::T_other_message :
            m_other_data = other_data_class(m_data_length);
            m_data_ptr = m_other_data.get_data_ptr();
            //m_data_length is set already in buffer
            break;
        case message_type_enum::T_camera_message :
            break;
        case message_type_enum::T_time_sync_message :
            break;
        case message_type_enum::T_undefined_message :
            break;
        }
}

message_class::~message_class(){
}

void message_class::print_meta_data(){
    std::cout << "\n\nHeader data:" << std::endl;
    std::cout << "m_message_type = " << m_message_type << std::endl;
    std::cout << "m_id = " << m_id << std::endl;
    std::cout << "m_data_length = " << m_data_length << std::endl;
    std::cout << "m_sensor_time_stamp = " << m_sensor_time_stamp << std::endl;
    std::cout << "m_data_ptr = " << (void *)m_data_ptr << std::endl;
    std::cout << "double check with obj = m_data_ptr = " << (void *)m_other_data.get_data_ptr() << std::endl;
}

void message_class::print_data(){
    std::cout << "Header data:" << m_other_data.get_data() << std::endl;
}

void message_class::print_data_buffer(const unsigned int count=10){
    unsigned int l = m_other_data.get_data_length();
    char *data_ptr = m_other_data.get_data_ptr();
    std::cout << "data buffer:      l = " << l << std::endl;
    unsigned int N = count<l?count:l;
    for (unsigned int i=0; i<N; i++){
        std::cout << i << "   " << (char)data_ptr[i] << std::endl;
    }
}


message_type_enum message_class::get_message_type(){
    return m_message_type;
}

unsigned int message_class::get_data_length(){
    return m_data_length;
}

char *message_class::get_data_ptr(){
    //return m_other_data.get_data_ptr();//m_data_ptr;
    return m_data_ptr;
}

other_data_class message_class::get_od_data(){
    return m_other_data;
}

//void message_class::add_data(data_buffer_class data_buffer){}

header_buffer_class message_class::populate_buffer_from_member_data(){
    // create the buffer and its references
    header_buffer_class buffer;
    unsigned int buffer_length = buffer.get_buffer_length();
    char *buffer_ptr = buffer.get_buffer_ptr();

    int pos = 0; // starting at beginning of buffer fill in the member data and increase pos for each step
    unsigned int count = 0; // keep count of bytes written to buffer

    //std::cout <<"message class : prepare header with metadata" << std::endl;
    unsigned int *ui_ptr = &m_id;
    int N = sizeof(m_id);
    std::copy( (char *)ui_ptr, (char *)ui_ptr+N, buffer_ptr );
    //std::memcpy(&m_header_buffer[pos], (char *)ui_ptr, N);
    pos+=N, count+=N; buffer_ptr+=N;

    ui_ptr = &m_data_length;
    N = sizeof(m_data_length);
    std::copy( (char *)ui_ptr, (char *)ui_ptr+N, buffer_ptr );
    //memcpy(&m_header_buffer[pos], (char *)ui_ptr, N);
    pos+=N, count+=N; buffer_ptr+=N;

    def_time_format *tf_ptr = &m_sensor_time_stamp;
    N = sizeof(m_sensor_time_stamp);
    std::copy( (char *)tf_ptr, (char *)tf_ptr+N, buffer_ptr );
    //memcpy(&m_header_buffer[pos], (char *)tf_ptr, N);
    pos+=N, count+=N; buffer_ptr+=N;

    message_type_enum *mt_ptr = &m_message_type;
    N = sizeof(message_type_enum);
    std::copy( (char *)mt_ptr, (char *)mt_ptr+N, buffer_ptr );
    //memcpy(&m_header_buffer[pos], (char *)mt_ptr, N);
    pos+=N, count+=N; buffer_ptr+=N;

    // the number of bytes written must fit in buffer size
    assert (count<=buffer_length);
    return buffer;
}

void message_class::populate_member_data_from_buffer(header_buffer_class &buffer){
    //std::cout <<"message class : extract meta data from headerbuffer" << std::endl;
    int pos = 0;    // index in buffer, increases by the item size
    unsigned int count = 0;  // running counter to check on appropriate index

    const unsigned int buffer_length = buffer.get_buffer_length();
    char *buffer_ptr = buffer.get_buffer_ptr();

    unsigned int *ui_ptr = &m_id;
    int N = sizeof(m_id);
    count+=N;
    assert (count<=buffer_length);
    std::copy( buffer_ptr, buffer_ptr + N , (char *)ui_ptr );
    //memcpy((char *)ui_ptr, &m_header_buffer[pos], N);
    pos+=N, buffer_ptr+=N;

    ui_ptr = &m_data_length;
    N = sizeof(m_data_length);
    count+=N;
    assert (count<=buffer_length);
    std::copy( buffer_ptr, buffer_ptr + N , (char *)ui_ptr );
    //memcpy((char *)ui_ptr, &m_header_buffer[pos], N);
    pos+=N, buffer_ptr+=N;

    def_time_format *tf_ptr = &m_sensor_time_stamp;
    N = sizeof(m_sensor_time_stamp);
    count+=N;
    assert (count<=buffer_length);
    std::copy( buffer_ptr, buffer_ptr + N , (char *)tf_ptr );
    //memcpy((char *)tf_ptr, &m_header_buffer[pos], N);
    pos+=N, buffer_ptr+=N;

    message_type_enum *mt_ptr = &m_message_type;
    N = sizeof(m_message_type);
    count+=N;
    assert (count<=buffer_length);
    std::copy( buffer_ptr, buffer_ptr + N , (char *)mt_ptr );
    //memcpy((char *)mt_ptr, &m_header_buffer[pos], N);
    pos+=N, buffer_ptr+=N;
}
