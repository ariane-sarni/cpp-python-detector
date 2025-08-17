#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

// new includes
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// simple struct for results
struct BBox {
    float xmin, ymin, xmax, ymax;
    float confidence;
    std::string name;
};


// returns json string. blocking, not ideal for perf but works
std::string sendFrame(const cv::Mat& frame) {
    // encode frame as jpg
    std::vector<uchar> buf;
    cv::imencode(".jpg", frame, buf);

    // create multipart form data
    cpr::Multipart multipart_data = {
        // "file" must match the name in the FastAPI endpoint
        {"file", cpr::Buffer{buf.begin(), buf.end(), "image/jpeg"}}
    };

    // send post request
    // make sure python server is running: uvicorn server:app --host 127.0.0.1 --port 8000
    cpr::Response r = cpr::Post(cpr::Url{"http://127.0.0.1:8000/detect/"},
                                multipart_data);

    if (r.status_code == 200) {
        return r.text;
    } else {
        std::cerr << "Error: " << r.status_code << " - " << r.text << std::endl;
        return "[]"; // return empty json array on error
    }
}


int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // NEW: send frame and get results
        std::string json_response = sendFrame(frame);
        
        // TODO: parse this json and draw boxes
        // just printing for now to prove it works
        std::cout << json_response << std::endl;


        cv::imshow("Webcam Feed", frame);
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    return 0;
}