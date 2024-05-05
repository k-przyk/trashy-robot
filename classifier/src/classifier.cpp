#include <iostream>
#include <zmq.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    zmq::context_t context(1);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5555"); // Connect to the sender's IP and port

    subscriber.set(zmq::sockopt::subscribe, ""); // Subscribe to all messages

    Mat image, hsvImage, mask;
    vector<vector<Point>> contours;

    while (true) {
        zmq::message_t message;
        subscriber.recv(message, zmq::recv_flags::none);

        // Convert the received message to a vector of bytes
        const char* message_data = static_cast<const char*>(message.data());
        vector<uchar> image_data(message_data, message_data + message.size());

        // Decode the image data
        image = imdecode(image_data, IMREAD_COLOR);

        // Convert the image from BGR to HSV color space
        cvtColor(image, hsvImage, COLOR_BGR2HSV);

        // Define the lower and upper bounds of hot pink color in HSV format
        Scalar lowerBound(160, 50, 50); // Lower bound for hot pink in HSV
        Scalar upperBound(180, 255, 255); // Upper bound for hot pink in HSV

        // Create a binary mask to identify hot pink pixels
        inRange(hsvImage, lowerBound, upperBound, mask);

        // Find contours in the mask
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // Find the largest hot pink object by area
        double maxArea = 0;
        int largestContourIndex = -1;

        for (size_t i = 0; i < contours.size(); i++) {
            double area = contourArea(contours[i]);
            if (area > maxArea) {
                maxArea = area;
                largestContourIndex = i;
            }
        }

        // Calculate the center point of the largest hot pink object
        Point2f centerPoint(-1, -1); // Initialize center point with an invalid value
        Rect boundingBox;

        if (largestContourIndex != -1) {
            Moments moments = cv::moments(contours[largestContourIndex]);
            if (moments.m00 > 0) {
                centerPoint = Point2f(static_cast<float>(moments.m10 / moments.m00), static_cast<float>(moments.m01 / moments.m00));
                boundingBox = boundingRect(contours[largestContourIndex]);
            }
        }

        // Draw the bounding box around the largest hot pink object
        if (centerPoint.x != -1 && centerPoint.y != -1) {
            rectangle(image, boundingBox, Scalar(0, 255, 0), 2); // Draw green rectangle
            circle(image, centerPoint, 5, Scalar(0, 255, 0), -1); // Draw green circle at center point
        }

        // Display the result with the bounding box and center point
        imshow("Hot Pink Object Detection", image);

        int key = waitKey(1);
        if (key == 'q') {
            break;
        }
    }

    return 0;
}
