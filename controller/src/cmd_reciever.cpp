#include "controller.hpp"
#include "motor.hpp"

int main() {

    int servoAngle, motorSpeed;

    MotorController motor(SERVO_CHANNEL, MOTOR_CHANNEL); 
    motor.setMotorSpeed(NEUTRAL); 
    motor.setServoAngle(STRAIGHT);

    zmq::context_t context(1);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5556"); // Connect to the sender's IP and port

    subscriber.set(zmq::sockopt::subscribe, ""); // Subscribe to all messages

    while (true) {
        zmq::message_t message;
        subscriber.recv(message, zmq::recv_flags::none);

        Command receivedCommand;
        memcpy(&receivedCommand, message.data(), sizeof(Command));

        std::cout << "Received Command: (" << receivedCommand.angle << ", " << receivedCommand.speed << ")" << std::endl;

        servoAngle = (int) ((receivedCommand.angle + 1) * STEERING_RANGE + MIN_STEERING); // Don't remove plus one
        motorSpeed = (int) receivedCommand.speed;
        motor.setMotorSpeed(motorSpeed);
        motor.setServoAngle(servoAngle);
    }

    return 0;
}