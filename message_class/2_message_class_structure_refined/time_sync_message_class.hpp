#ifndef _TIME_SYNC_MESSAGE_CLASS
#define _TIME_SYNC_MESSAGE_CLASS

#include <message_class.hpp>

/**
This class is derived from the message class.\n
It keeps several timestamps from a message roundtrip between the origin and a target (for global time management). \n
For tcp messging the class keeps a buffer and sets the buffer length and buffer address members of the base class. \n
In general, the derived class needs to implement how the buffer is populated with data and how it is extracted. \n
*/

class time_sync_message_class:public message_class{
private:
    //! buffer size, number of entries * size
    static const int m_buffer_length = 7 * sizeof(def_time_format);
    //! buffer for the timestamps
    def_time_format m_data_buffer[7];
public:
    //! timestamp when the data was observed
    def_time_format m_1_event_origin;
    //! timestamp when the data was sent at origin to target
    def_time_format m_2_tcp_send_origin;
    //! timestamp when the data was received at target
    def_time_format m_3_tcp_receipt_target;
    //! timestamp when the data was sent at target back to origin
    def_time_format m_4_tcp_send_target;
    //! timestamp when the data was received at origin
    def_time_format m_5_tcp_receipt_origin;
    //! timestamp when the data was sent at origin back to target
    def_time_format m_6_tcp_send_origin;
    //! timestamp when the data was received at target
    def_time_format m_7_tcp_receipt_target;

public:
    //! default constructor initializes all members to 0
    time_sync_message_class();
    //! default destructor
    ~time_sync_message_class();

    //! print all timestamps and the delta to previous
    void print_data();
    //! copies the data from the members into the buffer
    void prepare_data_buffer();
    //! copies the data from the buffer into the members
    void extract_data_buffer();
};

#endif
