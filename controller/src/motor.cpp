#include "motor.hpp" 

MotorController::MotorController(uint8_t servo_channel, uint8_t motor_channel) {
    pca = new PCA9685(1, DEVICE_ADDRESS); 
    pca->setPWMFreq(50); 
    servo = servo_channel; 
    motor = motor_channel; 
    m_speed = NEUTRAL; 
    s_angle = NEUTRAL; 
}

MotorController::~MotorController() {
    pca->setPWM(motor, 0); 
    pca->setPWM(servo, 0); 
    delete pca; 
    printf("Freeing MotorController Controller\n"); 
}

void MotorController::setMotorSpeed(int speed) {
    pca->setPWM(motor, speed); 
}

void MotorController::setServoAngle(int angle) {
    //Convert Angle to PWM signal. NOTE: Servo range = 1ms --> 2ms  
    pca->setPWM(servo, MIN_THROTTLE + angle); 
}

int MotorController::getMotorSpeed() {
   return pca->getPWM(motor); 
}

int MotorController::getServoAngle() {
    int pwm = pca->getPWM(servo); 
    return (pwm - MIN_THROTTLE); 
}

int MotorController::getPWMFrequency(int channel) {
    return pca->getPWM(channel); 
}

int MotorController::stepForwards() {
    m_speed += INCREMENT; 
    if(m_speed >= MAX_THROTTLE) { m_speed = MAX_THROTTLE; }

    pca->setPWM(motor, m_speed); 
}

int MotorController::stepBackwards() {
    m_speed -= INCREMENT; 
    if(m_speed <= MIN_THROTTLE) { m_speed = MIN_THROTTLE; }

    pca->setPWM(motor, m_speed); 
}

int MotorController::stepLeft() { // TODO: Double check how this is working
    s_angle += INCREMENT; 
    if(m_speed > MAX_THROTTLE) { return -1; }
    pca->setPWM(servo, s_angle); 
    return s_angle - MIN_THROTTLE;  
}

int MotorController::stepRight() {
    s_angle -= INCREMENT; 
    if(m_speed < MIN_THROTTLE) { return -1; }
    pca->setPWM(servo, s_angle); 
    return s_angle - MIN_THROTTLE;  
}





