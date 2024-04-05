#!/usr/bin/env python3

import time
from pathlib import Path
import cv2
import depthai as dai
import sys 

#Dim of 4k img
dim_x = 3840
dim_y = 2160 
counter = 0 


#Take in Downsize as an arg 
if len(sys.argv) != 2: 
    print("Missing Downsize Argument") 
    exit() 
downsize_factor = int(sys.argv[1])
downsize_dim_x = int(dim_x / (downsize_factor ** 2))  
downsize_dim_y = int(dim_y / (downsize_factor ** 2))  

# Create pipeline
pipeline = dai.Pipeline()

camRgb = pipeline.create(dai.node.ColorCamera)
camRgb.setBoardSocket(dai.CameraBoardSocket.CAM_A)
camRgb.setResolution(dai.ColorCameraProperties.SensorResolution.THE_4_K)

xoutRgb = pipeline.create(dai.node.XLinkOut)
xoutRgb.setStreamName("rgb")
camRgb.video.link(xoutRgb.input)

xin = pipeline.create(dai.node.XLinkIn)
xin.setStreamName("control")
xin.out.link(camRgb.inputControl)

# Properties
videoEnc = pipeline.create(dai.node.VideoEncoder)
videoEnc.setDefaultProfilePreset(1, dai.VideoEncoderProperties.Profile.MJPEG)
camRgb.still.link(videoEnc.input)

# Linking
xoutStill = pipeline.create(dai.node.XLinkOut)
xoutStill.setStreamName("still")
videoEnc.bitstream.link(xoutStill.input)

start = 0 
end = 0 
# Connect to device and start pipeline
with dai.Device(pipeline) as device:

    # Output queue will be used to get the rgb frames from the output defined above
    qRgb = device.getOutputQueue(name="rgb", maxSize=30, blocking=False)
    qStill = device.getOutputQueue(name="still", maxSize=30, blocking=True)
    qControl = device.getInputQueue(name="control")

    # Make sure the destination path is present before starting to store the examples
    dirName = f"rgb_data/{dim_x}x{dim_y}"
    downsizeDirName = f"rgb_data/{downsize_dim_x}x{downsize_dim_y}"
    Path(dirName).mkdir(parents=True, exist_ok=True)
    Path(downsizeDirName).mkdir(parents=True, exist_ok=True)

    while True:
        inRgb = qRgb.tryGet()  # Non-blocking call, will return a new data that has arrived or None otherwise
        if inRgb is not None:
            end = time.time() 
            print("Time: " + str(end - start)) 
            frame = inRgb.getCvFrame()
            # 4k / 4
            frame = cv2.pyrDown(frame)
            frame = cv2.pyrDown(frame)
            cv2.imshow("rgb", frame)
            start = time.time() 

        if qStill.has():
            img_time = int(time.time() * 1000) 
            fName = f"{dirName}/{img_time}.jpeg"
            fDownsize = f"{downsizeDirName}/{img_time}.jpeg"
            with open(fName, "wb") as f:
                f.write(qStill.get().getData()) 
                print("4K image saved to" + fName) 
            img = cv2.imread(fName) 
            for i in range(0,downsize_factor): 
                img = cv2.pyrDown(img) 
            cv2.imwrite(fDownsize,img) 
            print("Saved Downsize to " + fDownsize) 

        key = cv2.waitKey(1)
        if key == ord('q'):
            break
        elif key == ord('c'):
            ctrl = dai.CameraControl()
            ctrl.setCaptureStill(True)
            qControl.send(ctrl)
            counter += 1
            print("Counter: " + str(counter)) 
            print("Sent 'still' event to the camera!")
