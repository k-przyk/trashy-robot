import adafruit_pca9685
import board 
from adafruit_servokit import ServoKit
import busio 
import sys 
import time 

MIN_THROT=0x0ccc 
NEUTRAL=0x1333
MAX_THROT=0x1999 

class MotorController: 
    def __init__(self): 
        self.pwm = None 
        self.sc = None 
        self.speed = 0 
    def init_driver(self,hz = 50, channel = 14, servo_channel = 15): 
        i2c = busio.I2C(board.SCL, board.SDA)
        pca = adafruit_pca9685.PCA9685(i2c)
        pca.frequency = hz 
        self.pwm = pca.channels[channel] 
        self.sc = ServoKit(channels=16)  
        self.sc.servo[15].angle = 90
        self._angle = 90
        self.pwm.duty_cycle = NEUTRAL 
        self.speed = NEUTRAL
    def turn_right(self): 
        self._angle = self._angle + 1
        self.sc.servo[15].angle = self._angle
    def turn_left(self): 
        self._angle = self._angle - 1
        self.sc.servo[15].angle = self._angle
    def arm(self): 
        input(f"Setting to Neutral: %x\n",NEUTRAL) 
        self.pwm.duty_cycle = NEUTRAL 
        self.speed = NEUTRAL
    def set_speed(self,pwm): 
        self.speed = pwm 
    def max_throt(self): 
        self.speed = MAX_THROT 
        self.pwm.duty_cycle = self.speed 
    def min_throt(self): 
        self.speed = MIN_THROT 
        self.pwm.duty_cycle = self.speed 
    def fwd(self): 
        self.speed = self.speed + 0x010 
        self.pwm.duty_cycle = self.speed 
    def bck(self): 
        self.speed = self.speed - 0x010 
        self.pwm.duty_cycle = self.speed
    def print_speed(self): 
        print(self.speed)
    def exit(self): 
        self.pwm.duty_cycle = 0x0 
