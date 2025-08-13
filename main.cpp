#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0); // 0 = default camera
    if (!cap.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        cv::imshow("Webcam Feed", frame);

        // exit on 'q'
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    return 0;
}