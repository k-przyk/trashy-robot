#include <stdlib.h> 
#include <stdio.h> 
#include "mc.hpp"
#include <unistd.h> 
#include <ncurses.h> 

using namespace std; 

int main(int argc, char** argv){
    MC controller(16,15); 
    //printf("Motor Speed: %x\n",MAX_THROT); 
    controller.setMotorSpeed(NEUTRAL); 
    controller.setServoAngle(90); 
    char c; 
    initscr(); 
    c = getch(); 
    while(c != 'q'){
        c = getch(); 
        refresh(); 
        printw("Char: %c\n", c); 
        int out; 
        switch(c){
            case 'w': 
                out = controller.fwd(); 
                break; 
            case 'a': 
                out = controller.right(); 
                break; 
            case 's': 
                out = controller.bck(); 
                break; 
            case 'd': 
                out = controller.left(); 
                break; 

        }
        int pwm = controller.getPWMFreq(15); 
        printw("PWM signal on channel 15: %x\n", pwm); 
        printw("Speed/Angle: %d\n",out); 
        //Create a random number and toggle it 
    /*    int random = rand() % 3; 
        switch(random){
            case 0: 
            printf("PWM signal set: %x\n", MIN_THROT); 
            controller.setMotorSpeed(MIN_THROT); 
            break; 
            case 1: 
            printf("PWM signal set: %x\n", NEUTRAL); 
            controller.setMotorSpeed(NEUTRAL); 
            break; 
            case 2: 
            printf("PWM signal set: %x\n", MAX_THROT); 
            controller.setMotorSpeed(MAX_THROT); 
            break; 
        }
        int pwm = controller.getPWMFreq(15); 
        printf("PWM signal on channel 15: %x\n", pwm); 
        usleep(3 * microsecond); */
    }
    endwin(); 
    return 1; 
} 
