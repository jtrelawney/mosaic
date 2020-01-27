#include "opencv2/opencv.hpp"
#include <chrono>

cv::Mat make_copy(const cv::Mat &frame, const int &which_approach){

//1000 iterations
//taskset --cpu-list 1 ./create_mat.exe 
//average processing time , approach 0 = 185774[ns]
//average processing time , approach 1 = 1281425[ns]
//average processing time , approach 2 = 542[ns]


//10000 iterations
//taskset --cpu-list 1 ./create_mat.exe 
//average processing time , approach 0 = 124824[ns]
//average processing time , approach 1 = 335970[ns]
//average processing time , approach 2 = 511[ns]

    switch(which_approach){
        case 0: // just clone
            return frame.clone();
            break;
        case 1: // create empty mat, then memcopy
            {
            cv::Mat result = cv::Mat(cv::Size(frame.cols,frame.rows),frame.type(),cv::Scalar(128));
            uchar *source_buffer = frame.data;
            uchar *target_buffer = result.data;
            memcpy(target_buffer, source_buffer, frame.cols * frame.rows * frame.channels());
            return result;
            }
            break;
        case 2: // use mat constructor with direct buffer access
            return cv::Mat (cv::Size(frame.cols,frame.rows),frame.type(), frame.data);
            break;
    }
    
    // return empty matrix
    return cv::Mat(cv::Size(frame.cols,frame.rows),frame.type(),cv::Scalar(128));
}

int main(int, char**)
{
    cv::VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;

    cv::namedWindow("camera",1);
    cv::namedWindow("copy",1);

    // for each mat creation approach run counter iterations and measure the time
    for (int which_approach=0; which_approach<3; which_approach++){
        unsigned int acc_duration = 0, counter = 10000;
        for(unsigned int i=0; i<counter; i++)
        {
            cv::Mat frame;
            cap >> frame;

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            cv::Mat frame_copy = make_copy(frame,which_approach);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            //unsigned int duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
            unsigned int duration = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
            acc_duration+=duration;
            //std::cout << "Time difference = " << duration << "[µs]" << std::endl;
            //std::cout << "Time difference = " << duration << "[ns]" << std::endl;

            cv::imshow("camera", frame);
            cv::imshow("copy", frame_copy);
            if(cv::waitKey(30) >= 0) break;
        }
    std::cout << "average processing time , approach " << which_approach << " = " << acc_duration/counter << "[ns]" << std::endl;
    }

    return 0;
}
