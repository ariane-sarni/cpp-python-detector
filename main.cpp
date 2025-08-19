#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// simple struct for results
struct BBox {
    float xmin, ymin, xmax, ymax;
    float confidence;
    std::string name;
};

// new function to parse
std::vector<BBox> parseDetections(const std::string& json_str) {
    std::vector<BBox> boxes;
    try {
        auto j = json::parse(json_str);
        for (const auto& item : j) {
            BBox box;
            // use .at() to throw an exception if key is missing
            box.xmin = item.at("xmin");
            box.ymin = item.at("ymin");
            box.xmax = item.at("xmax");
            box.ymax = item.at("ymax");
            box.confidence = item.at("confidence");
            box.name = item.at("name");
            boxes.push_back(box);
        }
    } catch (json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    } catch (json::type_error& e) {
        std::cerr << "JSON type error: " << e.what() << std::endl;
    }
    return boxes;
}

// sendFrame function remains the same
std::string sendFrame(const cv::Mat& frame) {
    std::vector<uchar> buf;
    cv::imencode(".jpg", frame, buf);

    cpr::Multipart multipart_data = {
        {"file", cpr::Buffer{buf.begin(), buf.end(), "image/jpeg"}}
    };

    cpr::Response r = cpr::Post(cpr::Url{"http://127.0.0.1:8000/detect/"},
                                multipart_data,
                                cpr::Timeout{1000}); // 1s timeout

    if (r.status_code == 200) {
        return r.text;
    } else {
        // quiet fail
        // std::cerr << "Error: " << r.status_code << std::endl;
        return "[]"; 
    }
}

// new function to draw
void drawBoxes(cv::Mat& frame, const std::vector<BBox>& boxes) {
    for (const auto& box : boxes) {
        // draw rectangle
        cv::rectangle(frame, 
                        cv::Point(static_cast<int>(box.xmin), static_cast<int>(box.ymin)),
                        cv::Point(static_cast<int>(box.xmax), static_cast<int>(box.ymax)),
                        cv::Scalar(0, 255, 0), 2); // green, thickness 2
        
        // put label
        std::string label = box.name + " " + std::to_string(box.confidence).substr(0, 4);
        int baseLine;
        cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
        
        cv::rectangle(frame,
                        cv::Point(static_cast<int>(box.xmin), static_cast<int>(box.ymin) - labelSize.height - baseLine),
                        cv::Point(static_cast<int>(box.xmin) + labelSize.width, static_cast<int>(box.ymin)),
                        cv::Scalar(0, 255, 0), cv::FILLED);
                        
        cv::putText(frame, label, cv::Point(static_cast<int>(box.xmin), static_cast<int>(box.ymin) - baseLine),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1); // black text
    }
}

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return -1;
    }

    cv::namedWindow("Webcam Feed", cv::WINDOW_NORMAL);

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // 1. send frame
        std::string json_response = sendFrame(frame);
        
        // 2. parse response
        std::vector<BBox> boxes = parseDetections(json_response);
        
        // 3. draw boxes
        drawBoxes(frame, boxes);

        // 4. show result
        cv::imshow("Webcam Feed", frame);
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    return 0;
}