#include <iostream>
#include <memory>
#include <unistd.h> // usleep

#include <time_sync_message_class.hpp>
#include <text_message_class.hpp>

time_sync_message_class transfer(time_sync_message_class &ts_from){

    // tcp will work with message class and not derived
    message_class *tcp_message = &ts_from;

    // prepare data and header
    ts_from.prepare_data_buffer();
    tcp_message->populate_buffer_from_member_data();

    // grab buffer info
    unsigned int header_length = tcp_message->get_header_length();
    char *header_buffer_ptr = tcp_message->get_header_buffer_ptr();
    unsigned int data_length = tcp_message->get_data_length();
    char *data_buffer_ptr = tcp_message->get_data_buffer_ptr();

    // transfer to new message
    time_sync_message_class ts_to;

    // transfer header
    char *header_buffer_ptr_to = ts_to.get_header_buffer_ptr();
    memcpy(header_buffer_ptr_to,header_buffer_ptr,header_length);
    ts_to.populate_member_data_from_buffer();

    // transfer data
    char *data_buffer_ptr_to = ts_to.get_data_buffer_ptr();
    std::cout <<"m_data_length = " << data_length << std::endl;
    std::cout <<"data_buffer_ptr = " << (void *)data_buffer_ptr << std::endl;
    std::cout <<"data_buffer_target_ptr = " << (void *)data_buffer_ptr_to << std::endl;
    memcpy(data_buffer_ptr_to,data_buffer_ptr,data_length);
    ts_to.extract_data_buffer();

    return ts_to;
}

// sleeps for duration ms
void wait_a_while(int duration){
    usleep(duration);
}

void test_time_sync(){
    std::cout <<"creating time sync test message" << std::endl;
    time_sync_message_class ts1;
    ts1.print_header_data();

    std::cout <<"event at origin, processing and transfering" << std::endl;
    ts1.m_1_event_origin = get_time_stamp_ns();
    // processing time
    wait_a_while(500);
    ts1.m_2_tcp_send_origin = get_time_stamp_ns();
    ts1.print_data();
    time_sync_message_class ts2 = transfer(ts1);
    // transfer time
    wait_a_while(500);
    ts2.print_header_data();
    ts2.print_data();

    std::cout <<"receipt at target, processing and transfering back" << std::endl;
    ts2.m_3_tcp_receipt_target = get_time_stamp_ns();
    // processing time
    wait_a_while(500);
    ts2.m_4_tcp_send_target = get_time_stamp_ns();
    time_sync_message_class ts3 = transfer(ts2);
    // transfer time
    wait_a_while(500);
    ts3.print_header_data();
    ts3.print_data();

    std::cout <<"receipt at origin, processing and transfering back" << std::endl;
    ts3.m_5_tcp_receipt_origin = get_time_stamp_ns();
    // processing time
    wait_a_while(500);
    ts3.m_6_tcp_send_origin = get_time_stamp_ns();
    time_sync_message_class ts4 = transfer(ts3);
    // transfer time
    wait_a_while(500);
    ts4.m_7_tcp_receipt_target = get_time_stamp_ns();
    ts4.print_header_data();
    ts4.print_data();
}

void test_text_message(){

    std::cout <<"creating test message 1" << std::endl;
    text_message_class test_message1("thisisatest");
    test_message1.print_header_data();
    std::cout << "text = " << test_message1.get_data() << std::endl;

    // to play TCP: recreate message 1 from header data_length
    // 1. access message 1 as TCP would, using the base class, so that tcp can work without importing all message classes
    message_class *base_message;
    base_message = &test_message1;
    base_message->populate_buffer_from_member_data();
    char *buffer1 = base_message->get_header_buffer_ptr();
    test_message1.print_header_buffer_content();

    // 2. normally the original message class will be extracted from header and the proper
    std::cout <<"\n\ndetermine message class type and create corresponding empty message (not implemented)" << std::endl;

    // message type will be created, here we know it is a text message type
    // 3. create empty message2 and get its header buffer ptr
    std::cout <<"\n\ncreating empty test message 2 and get header buffer ptr" << std::endl;
    text_message_class test_message2;
    char *buffer2 = test_message2.get_header_buffer_ptr();

    // 3. copy the buffer
    std::cout <<"\n\ncopy header buffer from m1 to m2" << std::endl;
    unsigned int header_length = test_message2.get_header_length();
    for (unsigned int i=0; i<header_length; i++){
        //std::cout << int(buffer1[i]) << " , ";
        //std::cout << int(buffer2[i]) << " -> ";
        buffer2[i] = buffer1[i];
        //std::cout << int(buffer2[i]) << std::endl;
    }

    std::cout <<"\n\nextract header buffer to m2 members" << std::endl;
    test_message2.populate_member_data_from_buffer();
    test_message2.print_header_data();

    // 4. copy the data (text here)
    std::cout <<"\n\nmessage 1 - prepare data buffer" << std::endl;
    test_message1.prepare_data_buffer();

    std::cout <<"\n\nm1 get data length and ptr" << std::endl;
    unsigned int data_length = test_message1.get_data_length();
    buffer1 = test_message1.get_data_buffer_ptr();
    std::cout <<"data_length = " << data_length << std::endl;
    std::cout <<"buffer1 = " << (void *)buffer1 << std::endl;
    //for (unsigned int i=0; i<data_length; i++){ std::cout << buffer1[i] << std::endl;}

    // 4. copy the data (text here)
    std::cout <<"\n\nmessage 2 - prepare data buffer" << std::endl;
    std::cout <<"this checks whether the message is complete or if the header is available but not the data" << std::endl;
    test_message2.prepare_data_buffer();
    std::cout << "test message 2 , text = " << test_message2.get_data() << std::endl;

    std::cout <<"\n\nm2 get data ptr" << std::endl;
    buffer2 = test_message2.get_data_buffer_ptr();
    std::cout <<"buffer2 = " << (void *)buffer2 << std::endl;
    //for (unsigned int i=0; i<test_message2.get_data_length(); i++){ std::cout << buffer2[i] << std::endl;}

    // 3. copy the buffer
    std::cout <<"\n\ncopy data buffer from m1 to m2, N = " << data_length << std::endl;
    for (unsigned int i=0; i<data_length; i++){
        //std::cout << int(buffer1[i]) << " , ";
        //std::cout << int(buffer2[i]) << " -> ";
        buffer2[i] = buffer1[i];
        //std::cout << int(buffer2[i]) << std::endl;
    }


    // 5. display the result
    std::cout <<"\n\nmessage 2 should be complete now" << std::endl;
    std::cout << "test message 2 header info = " << std::endl;
    test_message2.print_header_data();
    std::cout << "text = " << test_message2.get_data() << std::endl;
}

int main(int argc, char *argv[])
{

    //test_time_sync();
    test_text_message();

    return 0;
}
