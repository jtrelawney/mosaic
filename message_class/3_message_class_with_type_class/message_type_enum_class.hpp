#ifndef _MESSAGE_TYPE_CLASS
#define _MESSAGE_TYPE_CLASS

enum class message_type_enum : unsigned char
{
    T_camera_message    = 0x01,
    T_time_sync_message = 0x02,
    T_other_message    = 0xfe,
    T_undefined_message = 0xff,
};

std::ostream& operator << (std::ostream& os, const message_type_enum& message_type)
{
    std::string result;
    switch(message_type){
        case(message_type_enum::T_camera_message):
            result = "camera message type";
            break;
        case(message_type_enum::T_time_sync_message):
            result = "time sync message type";
            break;
        case(message_type_enum::T_other_message):
            result = "other message type";
            break;
        case(message_type_enum::T_undefined_message):
            result = "undefined message type";
            break;

    }
    os << result;
    return os;
}

#endif
