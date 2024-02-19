#include "mc.hpp" 



void mc_testfunc(){
    printf("Gets into MC!\n"); 
}

MC::MC(int servo_channel, int motor_channel){
    pca = new PCA9685(1,ADDR); 
    pca->setPWMFreq(50); 
    servo = servo_channel; 
    motor = motor_channel; 
    m_speed = NEUTRAL; 
    s_angle = NEUTRAL; 
}

MC::~MC(){
    delete pca; 
}

void MC::setMotorSpeed(int speed){
    pca->setPWM(motor,speed); 
}
void MC::setServoAngle(int angle){
    //Convert Angle to PWM signal 
    //Servo range = 1ms --> 2ms 
    int angle_pwm = MIN_THROT + angle; 
    pca->setPWM(servo,angle_pwm); 
}

int MC::getMotorSpeed(){
   return pca->getPWM(motor); 
}

int MC::getServoAngle(){
    int pwm = pca->getPWM(servo); 
    return (pwm - MIN_THROT); 
}

int MC::getPWMFreq(int channel){
    return pca->getPWM(channel); 
}
void MC::fwd(){
    m_speed += INCR; 
    printf("Motor Speed: %x\n", m_speed); 
    pca->setPWM(motor,m_speed); 
}

void MC::bck(){
    m_speed -= INCR; 
    printf("Motor Speed: %x\n", m_speed); 
    pca->setPWM(motor,m_speed); 
}

void MC::right(){
    s_angle += INCR; 
    printf("Servo Angle: %d\n", (s_angle - MIN_THROT)); 
    pca->setPWM(servo,s_angle); 
}

void MC::left(){
    s_angle -= INCR; 
    printf("Servo Angle: %d\n", (s_angle - MIN_THROT)); 
    pca->setPWM(servo,s_angle); 
}





