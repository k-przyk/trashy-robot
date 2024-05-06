#include "controller.hpp" 
// #include "motor.hpp"

std::mutex mx;
Point objective;

void manuver(zmq::context_t *ctx);
void poll_objective(zmq::context_t *ctx);

float getAngleError(Point *trash_pt) {
    // Assuming that we have a point
    float a_error;

    // Get the pixel deviation from screen center
    a_error = (trash_pt->x - CENTER_X) / CENTER_X;
    return a_error;
}

float getDistanceError(Point *trash_pt) {
    // Assuming that we have a point
    float d_error;

    // Get the pixel deviation from screen center
    d_error = trash_pt->z;
    d_error = (d_error > SATURATE_DEPTH) ? SATURATE_DEPTH : d_error;
    d_error = d_error / SATURATE_DEPTH;
    return d_error;
}

// TODO: this should be threaded
void manuver(zmq::context_t *ctx) {

    // Controller variables
    float motor_speed = 0.0;
    float servo_angle = 0.0;

    // Gains
    float kp_angle = 1.0;
    float ki_angle = 0.0;
    float kd_angle = 0.0;

    float kp_speed = 1.0;
    float ki_speed = 0.0;
    float kd_speed = 0.0;

    // Timestep variables
    float a_error = 0.0; 
    float d_error = 0.0;
    float a_error_old = 0.0;
    float d_error_old = 0.0;
    float a_integral = 0.0;
    float d_integral = 0.0;
    float a_derivative = 0.0;
    float d_derivative = 0.0;
    float timestep = 0.005;
    int integral_steps = 0;

    zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
    publisher.bind("tcp://*:5556");

    Command commandToSend;
    Point trash_pt;

    while(true) {
        
        mx.lock();
            trash_pt = {objective.x, objective.y, objective.z};
        mx.unlock();

        // TODO: what if point values are -1?
        a_error = getAngleError(&trash_pt); 
        d_error = getDistanceError(&trash_pt);

        std::cout << "A Error: " << a_error << std::endl;
        
        // Handle Angle PID
        a_integral = a_integral + a_error * timestep;
        a_derivative = (a_error - a_error_old) / timestep;
        servo_angle = kp_angle * a_error + kd_angle * a_derivative + ki_angle * a_integral;

        // Handle Distance PID
        d_integral = d_integral + d_error * timestep;
        d_derivative = (d_error - d_error_old) / timestep;
        motor_speed = kp_speed * d_error + kd_speed * d_derivative + ki_speed * d_integral;

        // Prevent unbounded integral error accumulation
        if (integral_steps > 1000) { 
            a_integral = 0.0;
            d_integral = 0.0;
            integral_steps = 0;
        }

        // Break condition on driving to trash
        if (d_error < DISTANCE_TOLERANCE) {
            d_integral = 0;
            motor_speed = 0;
            // TODO: should do something? Or maybe stays stopped?
        }

        // Set next iteration values
        a_error_old = a_error;
        d_error_old = d_error;
        integral_steps++;

        // Command for motor
        servo_angle = (servo_angle < -1) ? -1 : servo_angle;
        servo_angle = (servo_angle >  1) ?  1 : servo_angle;
        motor_speed = (motor_speed >  1) ?  1 : motor_speed;
        commandToSend = {
            servo_angle, 
            motor_speed
        };

        zmq::message_t message(sizeof(Command));
        memcpy(message.data(), &commandToSend, sizeof(Command));
        publisher.send(message, zmq::send_flags::dontwait);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
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