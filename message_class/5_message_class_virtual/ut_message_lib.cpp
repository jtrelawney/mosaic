#include <iostream>
#include <message_class.hpp>
#include <time_class.hpp>

// example for deriving a message from the base class
// 1. constructors need to call the corresponding base class constructors and then set the own members
// 2. virtual function for populate_header_buffer_from_member_data():
//      first call the base class method to transfer those members into the buffer
//      then transfer the own members
// 3. virtual function for populate_member_data_from_buffer():
//      first call the base class method to transfer the buffer to those members
//      then transfer the own members
class test_message_class:public message_class{
private:
    int my_data_length;
    def_time_format my_time_stamp;
    std::string my_precious_data;
public:
    test_message_class(std::string text ,message_type_enum mtype, unsigned int id, def_time_format stamp=0):
        message_class(mtype,id),
        my_data_length(text.length()),
        my_time_stamp(stamp),
        my_precious_data(text)
    {
        std::cout <<"test message class constructor" << std::endl;
    };
    test_message_class(header_buffer_class &buffer):
        message_class(buffer),
        my_data_length(0),
        my_time_stamp(0)
    {
        std::cout <<"test message class constructor with buffer" << std::endl;
    };
    ~test_message_class(){};

    void print_meta_data(){
        message_class::print_meta_data();
        std::cout <<"my_data_length = " << my_data_length << std::endl;
        std::cout <<"my_time_stamp = " << my_time_stamp << std::endl;
        std::cout <<"my_precious_data = " << my_precious_data << std::endl;
    }

    buffer_info get_data_buffer_info(){
        buffer_info b;
        b.set_info(my_precious_data.length(),&*my_precious_data.begin());
        return b;
    };

    unsigned int populate_header_buffer_from_member_data(){
        // first call the populate function of the base class
        unsigned int N = message_class::populate_header_buffer_from_member_data();
        std::cout <<"writing header buffer for derived class at N = " << N << std::endl;
        // then copy the own members into the buffer
        N+=set_buffer_data(my_data_length, N);
        std::cout <<"N = " << N << std::endl;
        N+=set_buffer_data(my_time_stamp, N);
        std::cout <<"N = " << N << std::endl;
        print_header_buffer_content();
        return N;
    };

    unsigned int populate_member_data_from_buffer(){ //header_buffer_class &buffer
        // first call the populate function of the base class
        unsigned int N = populate_member_data(); //buffer
        std::cout <<"reading header buffer for derived class at N = " << N << std::endl;
        // then copy the buffer to own members
        N+=get_buffer_data(my_data_length, N);
        std::cout <<"N = " << N << std::endl;
        N+=get_buffer_data(my_time_stamp, N);
        std::cout <<"N = " << N << std::endl;
        print_header_buffer_content();
        return N;
    }

    // this prepares the data object so that the data can be received and written there
    void init_data_object_from_buffer_info(){
        std::cout <<"initializing data object with text length = " << my_data_length << std::endl;
        my_precious_data = std::string(my_data_length,'x');
        std::cout <<"text = " << my_precious_data << std::endl;
    }
};

// simulates the tcp receive process for the message header information
header_buffer_class receive_header_buffer(test_message_class message){
    // target buffer description
    header_buffer_class buffer;
    char *target_buffer = buffer.get_buffer_ptr();
    unsigned int target_length = buffer.get_buffer_length();

    // source buffer description
    buffer_info source_buffer_info = message.get_header_buffer_info();
    char *source_buffer = source_buffer_info.buffer_pointer;
    unsigned int source_length = source_buffer_info.buffer_length;

    // quick check
    if (target_length != source_length) std::cout << "buffer length mismatch" << std::endl;

    // copy over
    std::copy( source_buffer, source_buffer + target_length , target_buffer );
    return buffer;
}

// simulates the tcp receive process for data
void receive_data_buffer(message_class &source_message, message_class &target_message){

    // source and traget buffer info
    buffer_info source_buffer_info = source_message.get_data_buffer_info();
    buffer_info target_buffer_info = target_message.get_data_buffer_info();

    // some check
    if (source_buffer_info.buffer_length != target_buffer_info.buffer_length) std::cout << "buffer length mismatch" << std::endl;

    // copy over
    std::copy( source_buffer_info.buffer_pointer, source_buffer_info.buffer_pointer + source_buffer_info.buffer_length, target_buffer_info.buffer_pointer);
}

int main(){

    // decide on a type for the message test
    message_type_enum mtype = message_type_enum::T_camera_message;
    std::cout << "\n\ncreating test message" << std::endl;
    test_message_class test_message("test message",mtype,3,get_time_stamp_ns());
    test_message.print_meta_data();

    // populate the header buffer
    test_message.populate_header_buffer_from_member_data();

    // simulate a transfer, ie. copy the header buffer from the test object to a new object
    std::cout << "\n\nsend / receive header buffer" << std::endl;
    header_buffer_class buffer = receive_header_buffer(test_message);
    std::cout << "\n\nreceived this buffer" << std::endl;
    buffer.print_header_buffer_content();

    // then create the message from the buffer information
    message_type_enum received_mtype;
    buffer.get_buffer_data(received_mtype, 0);
    std::cout << "message type received in buffer = " << received_mtype << std::endl;

    if (mtype != received_mtype) std::cout << "message type mismatch" << std::endl;

    switch(received_mtype){
        case message_type_enum::T_other_message :
            std::cout << "final message received" << std::endl;
            break;
        case message_type_enum::T_camera_message :
            // use brackets to avoid - crosses initiliazation of .. error
            {
                std::cout << "creating test message from buffer" << std::endl;
                test_message_class received_message = test_message_class(buffer);
                received_message.populate_member_data_from_buffer();
                //received_message.print_meta_data();
                received_message.init_data_object_from_buffer_info();
                receive_data_buffer(test_message,received_message);
                std::cout << "final message received" << std::endl;
                received_message.print_meta_data();
            }
            break;
        case message_type_enum::T_time_sync_message :
            std::cout << "final message received" << std::endl;
            break;
        case message_type_enum::T_undefined_message :
            std::cout << "final message received" << std::endl;
            break;
        }
}
