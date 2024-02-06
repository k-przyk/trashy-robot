import adafruit_pca9685
import board 
import busio 
import sys 
import time 

MIN_THROT=0x07ff  
NEUTRAL=0x0bff 
MAX_THROT=0x0ff0 

class MotorController: 
    def __init__(self): 
        self.pwm = None 
        self.speed = 0 
    def init_driver(self,hz = 50, channel = 14): 
        i2c = busio.I2C(board.SCL, board.SDA)
        pca = adafruit_pca9685.PCA9685(i2c)
        pca.frequency = hz 
        self.pwm = pca.channels[channel] 
    def arm(): 
        input(f"Setting to Neutral: %x\n",NEUTRAL) 
        self.pwm.duty_cycle(NEUTRAL) 
        self.speed = NEUTRAL
    def set_speed(pwm): 
        self.speed = pwm 
    def max_throt(): 
        self.speed = MAX_THROT 
        self.pwm.duty_cycle(self.speed) 
    def min_throt(): 
        self.speed = MIN_THROT 
        self.pwm.duty_cycle(self.speed) 
    def fwd(): 
        self.speed = self.speed + 0x001 
        self.pwm.duty_cycle(self.speed) 
    def bck(): 
        self.speed = self.speed - 0x001 
        self.pwm.duty_cycle(self.speed) 
    def print_speed(): 
        print(self.speed)
    def exit(self): 
        self.pwm.duty_cycle = 0x0 
