#include <text_message_class.hpp>

text_message_class::text_message_class(){
}

text_message_class::text_message_class(std::string text):text_message_class(){
    // this is a full copy, do not use text to set the followign 2 members below, because text is temporary
    m_data_ptr = &*m_message_text.begin();
    m_message_text=text;
    m_data_length = m_message_text.size();
    m_sensor_time_stamp = get_time_stamp_ns();
    std::cout <<"test message class: constructor , id = " << get_id() << " , data length = " << get_data_length() << std::endl;
    std::cout <<"test message class: constructor , text = " << m_message_text << std::endl;
    //for (unsigned int i=0; i<m_data_length; i++){ std::cout << m_data_ptr[i] << std::endl;}
}

void text_message_class::prepare_data_buffer(){
    std::cout <<"testmessageclass - prepare data buffer()" << std::endl;
    unsigned int data_length = m_message_text.length();
    if (m_data_length == data_length) {
        std::cout <<"when text is stored in message the datalength and ptr are set to the string" << std::endl;
        std::cout <<"m_data_length = " << m_data_length << "   , ptr = " << (void *) m_data_ptr << "   text = " << std::endl;
        //for (unsigned int i=0; i<m_data_length; i++){ std::cout << m_data_ptr[i] << std::endl;}
    } else {
        std::cout << "data_length doesn't match stored text -> header ready and data buffer to be created" << std::endl;
        m_message_text = std::string(m_data_length,'x');
        m_data_ptr = &*m_message_text.begin();
    }
}

void text_message_class::extract_data_buffer(){
    std::cout <<"testmessageclass - extract data buffer()" << std::endl;
    std::cout <<"when buffer is prepared the members are updated and then the buffer is written directly - nothging to do" << std::endl;
}

void text_message_class::set_data(std::string text){
    m_message_text = text;
    m_data_ptr = &*m_message_text.begin();
    m_data_length = m_message_text.size();
}
std::string text_message_class::get_data(){ return m_message_text; }
text_message_class::~text_message_class(){
    std::cout <<"test message class: de-structor" << std::endl;
}
