#include "mc.hpp" 



void mc_testfunc(){
    printf("Gets into MC!\n"); 
}

MC::MC(uint8_t servo_channel, uint8_t motor_channel){
    pca = new PCA9685(1,ADDR); 
    pca->setPWMFreq(50); 
    servo = servo_channel; 
    motor = motor_channel; 
    m_speed = NEUTRAL; 
    s_angle = NEUTRAL; 
}

MC::~MC(){
    pca->setPWM(motor,0); 
    pca->setPWM(servo,0); 
    delete pca; 
    printf("Freeing MC Controller\n"); 
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
int MC::fwd(){
    m_speed += INCR; 
    pca->setPWM(motor,m_speed); 
    return m_speed; 
}

int MC::bck(){
    m_speed -= INCR; 
    pca->setPWM(motor,m_speed); 
    return m_speed; 
}

int MC::right(){
    s_angle += INCR; 
    pca->setPWM(servo,s_angle); 
    return s_angle - MIN_THROT;  
}

int MC::left(){
    s_angle -= INCR; 
    pca->setPWM(servo,s_angle); 
    return s_angle - MIN_THROT;  
}





