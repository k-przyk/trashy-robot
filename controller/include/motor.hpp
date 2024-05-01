#pragma once
#include <cstdint>
#include <stdlib.h> 
#include <stdio.h> 
// #include "PCA9685.h"

// Important Constants
// Speed
#define NEUTRAL 0X133 
#define MIN_THROTTLE 0X0cc 
#define MAX_THROTTLE 0x199 
#define THROTTLE_RANGE (MAX_THROT - MIN_THROT)
#define FREQUENCY 50 
#define INCREMENT 0X1 

// Device State
#define DEVICE_ADDRESS 0x40 // Default address for i2c driver
#define SERVO_CHANNEL 16
#define MOTOR_CHANNEL 15

//Store internally as 12 bit resolution
// Class to manipulate rc car motor speed
class MotorController{
    public: 
        // Controller Object to change Motor PWM
        MotorController(uint8_t, uint8_t); // Takes in Servo and ESC channels
        ~MotorController(); 
        void setMotorSpeed(int speed); 
        void setServoAngle(int angle); 
        int stepForwards(); 
        int stepBackwards(); 
        int stepRight(); 
        int stepLeft(); 
        int getMotorSpeed(); 
        int getServoAngle(); 
        int getPWMFrequency(int); 

    private: 
        // Class variables
        // PCA9685* pca; // Motor Driver
        int motor; 
        int servo; 
        int m_speed; 
        int s_angle; 
}; 