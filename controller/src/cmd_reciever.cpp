#include "controller.hpp"
#include "motor.hpp"

#define AVERAGE_LENGTH 15

int main() {

    int angles[AVERAGE_LENGTH];
    int speeds[AVERAGE_LENGTH];
    int servoAngle, motorSpeed, index, nextAngle, nextSpeed;

    MotorController motor(SERVO_CHANNEL, MOTOR_CHANNEL); 
    motor.setMotorSpeed(NEUTRAL); 
    motor.setServoAngle(STRAIGHT);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    std::cout << "Starting!" << std::endl;

    zmq::context_t context(1);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5556"); // Connect to the sender's IP and port

    subscriber.set(zmq::sockopt::subscribe, ""); // Subscribe to all messages

    // Init running averages
    for (int i = 0; i < AVERAGE_LENGTH; i++) {
        angles[i] = 0;
        speeds[i] = 0;
    }

    while (true) {
        zmq::message_t message;
        subscriber.recv(message, zmq::recv_flags::none);

        Command receivedCommand;
        memcpy(&receivedCommand, message.data(), sizeof(Command));

        std::cout << "Received Command: (" << receivedCommand.angle << ", " << receivedCommand.speed << ")" << std::endl;

        int nextAngle = (int) ((receivedCommand.angle / 2.0 + 0.5) * STEERING_RANGE + MIN_STEERING); // Don't remove plus one
        int nextSpeed = (int) (receivedCommand.speed * THROTTLE_RANGE + MIN_THROTTLE);

        servoAngle -= angles[index % AVERAGE_LENGTH];
        servoAngle += nextAngle;
        angles[index % AVERAGE_LENGTH] = nextAngle;

        motorSpeed -= speeds[index % AVERAGE_LENGTH];
        motorSpeed += nextSpeed;
        speeds[index % AVERAGE_LENGTH] = nextSpeed;

        std::cout << "Setting Motor Speed: " << motorSpeed << " Servo Angle: " << servoAngle << std::endl;
        motor.setMotorSpeed(motorSpeed / AVERAGE_LENGTH);
        motor.setServoAngle(servoAngle / AVERAGE_LENGTH);

        index = ++index >= AVERAGE_LENGTH ? 0 : index;
    }

    return 0;
}