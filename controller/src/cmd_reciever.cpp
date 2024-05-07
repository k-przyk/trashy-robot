#include "controller.hpp"
#include "motor.hpp"

#define AVERAGE_LENGTH 7

int main() {

    int speeds[AVERAGE_LENGTH];
    int index, nextSpeed, motorSum, motorSpeed, servoAngle;

    MotorController motor(SERVO_CHANNEL, MOTOR_CHANNEL); 
    motor.setMotorSpeed(NEUTRAL); 
    motor.setServoAngle(STRAIGHT);

    zmq::context_t context(1);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5556"); // Connect to the sender's IP and port

    subscriber.set(zmq::sockopt::subscribe, ""); // Subscribe to all messages

    // Init running averages
    for (int i = 0; i < AVERAGE_LENGTH; i++) {
        speeds[i] = MIN_THROTTLE;
    }
    motorSum = MIN_THROTTLE * AVERAGE_LENGTH;

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::cout << "Starting!" << std::endl;

    while (true) {
        zmq::message_t message;
        subscriber.recv(message, zmq::recv_flags::none);

        Command receivedCommand;
        memcpy(&receivedCommand, message.data(), sizeof(Command));

        std::cout << "Received Command: (" << receivedCommand.angle << ", " << receivedCommand.speed << ")" << std::endl;

        servoAngle = (int) ((receivedCommand.angle / 2.0 + 0.5) * STEERING_RANGE + MIN_STEERING); // Don't remove plus one
        nextSpeed = (int) (receivedCommand.speed * THROTTLE_RANGE + MIN_THROTTLE);

        motorSum -= speeds[index % AVERAGE_LENGTH];
        speeds[index % AVERAGE_LENGTH] = nextSpeed;
        motorSum += speeds[index % AVERAGE_LENGTH];
        motorSpeed = motorSum / AVERAGE_LENGTH;

        std::cout << "Setting Motor Speed: " << motorSpeed << " Servo Angle: " << servoAngle << std::endl;
        motor.setMotorSpeed(motorSpeed);
        motor.setServoAngle(servoAngle);
    }

    return 0;
}