from adafruit_servokit import ServoKit
kit = ServoKit(channels=16)
kit.servo[15].angle = 180
input("wait until I am done 180") 
kit.servo[15].angle = 90 
input("wait until i am done 90") 
kit.servo[15].angle = 0
