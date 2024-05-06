#include "classifier.hpp"

using namespace std;
using namespace cv;

int main() {

    Mat image, hsvImage, mask;
    vector<vector<Point>> contours;

    Scalar lowerBound(160, 50, 50); 
    Scalar upperBound(180, 255, 255); 
    Rect boundingBox;

    zmq::context_t context(1);

    zmq::socket_t img_subscriber(context, zmq::socket_type::sub);
    img_subscriber.connect("tcp://localhost:5553");
    img_subscriber.set(zmq::sockopt::subscribe, "");

    zmq::socket_t roi_subscriber(context, zmq::socket_type::sub);
    roi_subscriber.connect("tcp://localhost:5554");
    roi_subscriber.set(zmq::sockopt::subscribe, "");

    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.bind("tcp://*:5555");

    cout << "Starting!" << endl;

    int depthValue = 0;

    while (true) {
        zmq::message_t imageMessage;
        img_subscriber.recv(imageMessage, zmq::recv_flags::none);

        zmq::message_t depthMessage;
        if (roi_subscriber.recv(&depthMessage, ZMQ_DONTWAIT)) {
            memcpy(&depthValue, depthMessage.data(), sizeof(int));
        }

        // Decode image
        const char* messageData = static_cast<const char*>(imageMessage.data());
        vector<uchar> imageData(messageData, messageData + imageMessage.size());
        image = imdecode(imageData, IMREAD_COLOR);

        // Convert to HSV
        cvtColor(image, hsvImage, COLOR_BGR2HSV);
        inRange(hsvImage, lowerBound, upperBound, mask);

        // Find contours
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // Find the largest object
        double maxArea = 0;
        int largestContourIndex = -1;

        for (size_t i = 0; i < contours.size(); i++) {
            double area = contourArea(contours[i]);
            if (area > maxArea) {
                maxArea = area;
                largestContourIndex = i;
            }
        }

        // Find center of object in frame
        Point2f centerPoint(-1, -1);

        if (largestContourIndex != -1) {
            Moments moments = cv::moments(contours[largestContourIndex]);
            if (moments.m00 > 0) {
                centerPoint = Point2f(static_cast<float>(moments.m10 / moments.m00), static_cast<float>(moments.m01 / moments.m00));
                boundingBox = boundingRect(contours[largestContourIndex]);
            }
        }

        // Draw bounding box
        if (centerPoint.x != -1 && centerPoint.y != -1) {
            rectangle(image, boundingBox, Scalar(0, 255, 0), 2);
            circle(image, centerPoint, 5, Scalar(0, 255, 0), -1);
        }

        imshow("Object Detection", image);

        int key = waitKey(1);
        if (key == 'q') break;

        CommandPoint objective = {centerPoint.x, centerPoint.y, (float) depthValue};

        cout << "Center - x: " << objective.x << " y: " << objective.y << " z: " << depthValue << endl;

        zmq::message_t msg(sizeof(CommandPoint));
        memcpy(msg.data(), &objective, sizeof(CommandPoint));
        publisher.send(msg, zmq::send_flags::dontwait);
    }

    return 0;
}
