#include <test_message_class.h>

test_message_class::test_message_class(){
}
test_message_class::test_message_class(std::string text):test_message_class(){
    m_message_text=text;
    m_message_data_length = text.size();
    std::cout <<"test message class: constructor , id = " << m_id << " , data length = " << m_message_data_length << std::endl;
    std::cout <<"test message class: constructor , text = " << m_message_text << std::endl;
}

void test_message_class::set_data(std::string text){ m_message_text = text;}
std::string test_message_class::get_data(){ return m_message_text; }
test_message_class::~test_message_class(){
    std::cout <<"test message class: de-structor" << std::endl;
}
