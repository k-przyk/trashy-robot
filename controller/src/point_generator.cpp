#include "controller.hpp"

float randomFloat(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

int main() {
    int step = 0;
    int maxStep = 5; // 5 seconds then switch
    float x, y, z;

    zmq::context_t context(1);
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.bind("tcp://*:5555"); // Bind to port 5555

    Point pointToSend;

    while (true) {

        // Generate new point
        if (step % maxStep == 0) {
            x = randomFloat(0.0, 1.0);
            y = randomFloat(0.0, 1.0);
            z = randomFloat(0.0, 1.0);
            pointToSend = {x, y, z};
            step = 0;
        }

        zmq::message_t message(sizeof(Point));
        memcpy(message.data(), &pointToSend, sizeof(Point));
        publisher.send(message, zmq::send_flags::dontwait);

        std::cout << "Sent Point: (" << pointToSend.x << ", " << pointToSend.y << ", " << pointToSend.z << ")" << std::endl;

        // Simulate sending points periodically
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        step++;
    }

    return 0;
}