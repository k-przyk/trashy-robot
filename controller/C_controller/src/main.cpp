#include <stdlib.h> 
#include <stdio.h> 
#include "mc.hpp"

int main(int argc, char** argv){
    MC controller(15,14); 
    printf("Motor Speed: %x\n",MAX_THROT); 
    controller.setMotorSpeed(MIN_THROT); 
    return 1; 
} 
