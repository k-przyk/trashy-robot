#include "controller.hpp" 

std::mutex mx;
Point objective;

void manuver(zmq::context_t *ctx);
void poll_objective(zmq::context_t *ctx);

// float getAngleError(Point *trash_pt) {
//     // Assuming that we have a point
//     float a_error;

//     // Check if we can reach this
//     if (trash_pt->y < REACHABLE_Y) { // TODO: AND with other field conditions
//         return -1;
//     }

//     // Get the pixel deviation from screen center
//     a_error = (trash_pt->y - CENTER_X) / CENTER_X;
//     return a_error;
// }

// float getDistanceError(Point *trash_pt) {
//     // Assuming that we have a point
//     float d_error;

//     // Check if we can reach this
//     if (trash_pt->y < REACHABLE_Y) { // TODO: AND with other field conditions
//         return -1;
//     }

//     // Get the pixel deviation from screen center
//     d_error = trash_pt->z;
//     return d_error;
// }

// TODO: this should be threaded
void manuver(zmq::context_t *ctx) {

    // Controller variables
    int motor_speed = 0;
    int servo_angle = 0;

    // Gains
    float kp_angle = 8000;
    float ki_angle = 0;
    float kd_angle = 0;

    float kp_speed = 3000;
    float ki_speed = 150;
    float kd_speed = 50;

    // Timestep variables
    float aerror = 0.0; 
    float derror = 0.0;
    float derror_old = 0.0;
    float a_integral = 0.0;
    float d_integral = 0.0;
    float a_derivative = 0.0;
    float d_derivative = 0.0;
    float timestep = 0.01;
    int integral_steps = 0;

    zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
    publisher.bind("tcp://*:5556");

    Command commandToSend;

    while(true) {
        
        mx.lock();
            commandToSend = {objective.x, objective.y};
        mx.unlock();

        // a_error = getAngleError(trash_pt); 
        // if a_error == -1: // If the trash is not reachable or not seen, do something else
        //     return; // TODO: What is the default behavior on model non-recognition?

        // d_error = getDistanceError(trash_pt);
        
        // // Handle Angle PID
        // a_integral = a_integral + a_error * timestep;
        // a_derivative = (a_error - a_error_old) / timestep;
        // servo_angle = kp_angle * a_error + kd_angle * a_derivative + ki_angle + a_integral

        // // Handle Distance PID
        // d_integral = d_integral + d_error * timestep;
        // d_derivative = (d_error - d_error_old) / timestep;
        // motor_speed = kp_speed * d_error + kd_speed * d_derivative + ki_speed * d_integral;
        // motor_output = int(motor_speed * THROTTLE_RANGE + MIN_THROTTLE) // TODO: remove the 4

        // // Prevent unbounded integral error accumulation
        // if (integral_steps > 1000) { 
        //     a_integral = 0.0;
        //     d_integral = 0.0;
        //     integral_steps = 0;
        // }

        // // Break condition on driving to trash
        // if (derror < DISTANCE_BOUND) {
        //     d_integral = 0;
        //     motor_speed = 0;
        //     // TODO: should do something? Or maybe stays stopped?
        // }

        // // Affect Controller
        // motor.setMotorSpeed(motor_output)
        // motor.setServoAngle(servo_output)

        // // Set next iteration values
        // a_error_old = a_error;
        // d_error_old = d_error;
        // integral_steps++;

        zmq::message_t message(sizeof(Command));
        memcpy(message.data(), &commandToSend, sizeof(Command));
        publisher.send(message, zmq::send_flags::dontwait);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void poll_objective(zmq::context_t *ctx) {

    // Start the subscriber
    zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5555");

    subscriber.set(zmq::sockopt::subscribe, "");

    // Run until process is killed
    while (true) {
        zmq::message_t message;
        subscriber.recv(message, zmq::recv_flags::none);

        Point receivedPoint;
        memcpy(&receivedPoint, message.data(), sizeof(Point));

        // Populate shared point for PID loop
        mx.lock();
            objective = receivedPoint;
        mx.unlock();
    }
}

int main() {

    // Retrieve ZMQ context
    zmq::context_t context(1);

    // Init Values for the objective
    mx.lock(); // Lock not strictly needed here, but left in case of compiler optimization
        objective = {0.0f, 0.0f};
    mx.unlock();

    // Start the PID loop
    auto controller = std::async(std::launch::async, manuver, &context);
    auto poller = std::async(std::launch::async, poll_objective, &context);

    // Clean exit condition
    return 0;
}