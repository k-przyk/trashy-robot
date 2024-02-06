import adafruit_pca9685
from adafruit_servokit import ServoKit
import board 
import busio 
import sys 
import time 


#Ranges out the throttle
def arm(): 
    print("Arming") 
    input("Full Throttle") 
    motor_channel.duty_cycle = 0x1ff0
    #kit.servo[14].angle = 180 
    input("Neutral") 
#    motor_channel.duty_cycle = 0x1c30
    motor_channel.duty_cycle = 0x0ff0
    #kit.servo[14].angle = 0

def main(): 
    print("Starting motor test") 
    if sys.argv[1] == '1':
        arm() 
    input("Enter to run for 5 seconds") 
    motor_channel.duty_cycle = 0x1b30
    time.sleep(5)
    print("Finished test") 
    motor_channel.duty_cycle = 0x0



if  __name__ == "__main__": 
    i2c = busio.I2C(board.SCL, board.SDA)
    pca = adafruit_pca9685.PCA9685(i2c)
    pca.frequency = 50 
    motor_channel = pca.channels[14] 
    motor_channel.duty_cycle = 0x0ff0
    #kit = ServoKit(channels=16)
    #Determine if calibration is needed store it in the first arg 
    if len(sys.argv) != 2: 
        print("Need Calibration arg") 
        exit() 
    main() 

