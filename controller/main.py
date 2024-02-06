from motor_controller import MotorController
import os 

def main(): 
    mc = MotorController() 
    mc.init_driver() #Defaults to 50, channel to 14  
    if sys.argv[1] == 1: 
        mc.arm() 
    mc.print_speed() 




if  __name__ == "__main__": 
    #Determine if calibration is needed store it in the first arg 
    if len(sys.argv) != 2: 
        print("Need Calibration arg") 
        exit() 
    main() 
