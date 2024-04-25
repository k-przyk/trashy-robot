#include <linux/interrupt.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>

#include "motor_controller.hpp" 

void manuver(void);
static void timer_init(void);
static void timer_cleanup(void);

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
int16_t integral_steps = 0;

// Sensor values 
// TODO: Populate any sensor values here

// interrupt
static struct hrtimer htimer;
static ktime_t kt_periode;

static void timer_init(void)
{
    kt_periode = ktime_set(0, 104167); //seconds,nanoseconds
    hrtimer_init (& htimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
    htimer.function = timer_function;
    hrtimer_start(& htimer, kt_periode, HRTIMER_MODE_REL);
}

static void timer_cleanup(void)
{
    hrtimer_cancel(& htimer);
}

static enum hrtimer_restart timer_function(struct hrtimer * timer)
{
    // Compute PID values here
    manuver();

    // Timer Restart
    hrtimer_forward_now(timer, kt_periode);
    return HRTIMER_RESTART;
}

void manuver(void) {

    // Timestep variables
    a_error = getAngleError(); 
    d_error = getDistanceError();
    
    // Handle Angle PID
    a_integral = a_integral + a_error * timestep;
    a_derivative = (a_error - a_error_old) / timestep;
    servo_angle = kp_angle * a_error + kd_angle * a_derivative + ki_angle + a_integral
    angle_output = int(servo_angle * THROTTLE_RANGE / 4 + MIN_THROTTLE)

    // Handle Distance PID
    d_integral = d_integral + d_error * timestep;
    d_derivative = (d_error - d_error_old) / timestep;
    motor_speed = kp_speed * d_error + kd_speed * d_derivative + ki_speed * d_integral;
    motor_output = int(motor_speed * THROTTLE_RANGE + MIN_THROTTLE) // TODO: remove the 4

    // Prevent unbounded integral error accumulation
    if (integral_steps > 1000) { 
        a_integral = 0.0;
        d_integral = 0.0;
        integral_steps = 0;
    }

    // Break condition on driving to trash
    if (derror < DISTANCE_BOUND) {
        d_integral = 0;
        motor_speed = 0;
        // TODO: should do something? Or maybe stays stopped?
    }

    // Affect Controller
    motor.setMotorSpeed(motor_output)
    motor.setServoAngle(servo_output)

    // Set next iteration values
    a_error_old = a_error;
    d_error_old = d_error;
    integral_steps++;
}