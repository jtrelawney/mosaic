#include <cstring> // for memcpy
#include <iostream> // for cout
#include <assert.h> // for assert
#include <algorithm>    // std::copy


#include <message_class.hpp>
#include <message_type_enum_class.hpp>

message_class transfer(message_class message){

    // extract the header buffer and create a new message from it
    header_buffer_class buffer = message.populate_buffer_from_member_data();
    //buffer.print_header_buffer_content();
    // tcp send here
    // tcp receive here
    message_class new_message(buffer);

    // get the data buffer info from the source
    char *odptr = message.get_data_ptr();
    unsigned int odlength = message.get_data_length();
    // tcp send here

    // get the data buffer info from the target
    char *ndptr = new_message.get_data_ptr();
    unsigned int dlength = new_message.get_data_length();
    // tcp receive here

    // quickly double check
    if (dlength!=odlength) std::cout << "text lengths don't match" << std::endl;

    // copy the data, for tcp receive this would just fill the data into the given ptr
    std::copy(odptr, odptr + odlength, ndptr);

    return new_message;
}

int main(int argc, char *argv[])
{
    std::cout << std::endl << "create test message:" << std::endl;
    message_class test_message(2,other_data_class());
    //test_message.print_meta_data();
    test_message.print_data();

    std::cout << std::endl << "copy the test message:" << std::endl;
    message_class copied_message = transfer(test_message);
    //copied_message.print_meta_data();

    std::cout << std::endl << "extract the data object:" << std::endl;
    other_data_class od = copied_message.get_od_data();
    std::string text = od.get_data();
    std::cout << "text  = " << text << std::endl;

    return 0;
}
