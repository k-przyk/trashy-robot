#pragma once
#include <stdlib.h> 
#include <stdio.h> 
#include "PCA9685.h"



//Store internally as 12 bit resolution

#define MIN_THROT 0X0cc 
#define NEUTRAL 0X133 
#define MAX_THROT 0x199 
#define FREQ 50 
#define INCR 0X1 
#define ADDR 0x40 


void mc_testfunc(); 

class MC{
    public: 
        MC(int,int); //Servo + ESC channel 
        ~MC(); 
        void fwd(); 
        void bck(); 
        void right(); 
        void left(); 
        int getMotorSpeed(); 
        int getServoAngle(); 
        void setMotorSpeed(int speed); 
        void setServoAngle(int angle); 
        int getPWMFreq(int); 
    private: 
        PCA9685* pca; 
        int motor; 
        int servo; 
        int m_speed; 
        int s_angle; 
}; 
