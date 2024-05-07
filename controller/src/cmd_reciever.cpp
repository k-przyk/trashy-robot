#include "controller.hpp"
#include "motor.hpp"

#define AVERAGE_LENGTH 7

int main() {

    int speeds[AVERAGE_LENGTH];
    int index, motorSpeed, nextSpeed, angleOutput, speedOutput;

    MotorController motor(SERVO_CHANNEL, MOTOR_CHANNEL); 
    motor.setMotorSpeed(NEUTRAL); 
    motor.setServoAngle(STRAIGHT);

    zmq::context_t context(1);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5556"); // Connect to the sender's IP and port

    subscriber.set(zmq::sockopt::subscribe, ""); // Subscribe to all messages

    // Init running averages
    for (int i = 0; i < AVERAGE_LENGTH; i++) {
        speeds[i] = 0;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "Starting!" << std::endl;

    while (true) {
        zmq::message_t message;
        subscriber.recv(message, zmq::recv_flags::none);

        Command receivedCommand;
        memcpy(&receivedCommand, message.data(), sizeof(Command));

        std::cout << "Received Command: (" << receivedCommand.angle << ", " << receivedCommand.speed << ")" << std::endl;

        angleOutput = (int) ((receivedCommand.angle / 2.0 + 0.5) * STEERING_RANGE + MIN_STEERING); // Don't remove plus one
        nextSpeed = (int) (receivedCommand.speed * THROTTLE_RANGE + MIN_THROTTLE);

        motorSpeed -= speeds[index % AVERAGE_LENGTH];
        motorSpeed += nextSpeed;
        speeds[index % AVERAGE_LENGTH] = nextSpeed;
        speedOutput = motorSpeed / AVERAGE_LENGTH;

        std::cout << "Setting Motor Speed: " << speedOutput << " Servo Angle: " << angleOutput << std::endl;
        motor.setMotorSpeed(speedOutput);
        motor.setServoAngle(angleOutput);
    }

    return 0;
}