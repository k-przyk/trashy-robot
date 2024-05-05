#!/usr/bin/env python3
import numpy as np
import math
import zmq
import argparse 
import cv2
import torchvision 
import torch 
import torchvision.transforms as transforms 
from PIL import Image 
from datetime import timedelta

FPS = 30 
baseline = 75 
fov = 71.86
width = 640
focal = width / (2*math.tan(fov / 2 / 180 * math.pi)) 

def display_bbox(img, boxes, scores):
    if boxes is None or scores is None: 
        return
    for bbox,score in zip(boxes,scores): 
        xmin = bbox[0].item() / 320.0 * 960.0
        ymin = bbox[1].item() / 320.0 * 540.0
        xmax = bbox[2].item() / 320.0 * 960.0 
        ymax = bbox[3].item() / 320.0 * 540.0
        #Need to resize these poitns 
        pt1 = (int(xmin),int(ymin))
        pt2 = (int(xmax),int(ymax))
        cv2.rectangle(img, pt1, pt2, (0,255,0),2)
        cv2.putText(img,str(score.item()),pt1,cv2.FONT_HERSHEY_SIMPLEX, 0.9, (36,255,12),2)
        return (pt1,pt2) 

def model_inference(img, device, image_transforms, model, threshold=0.65): 
    tensor = image_transforms(img).to(device)  
    output = model([tensor]) 
    scores = output[0]['scores'] 
    boxes = output[0]['boxes']  
    high_score_indices = scores > threshold
    return (boxes[high_score_indices],scores[high_score_indices]) 

def run(args): 
    # PyTorch 
    model = torchvision.models.detection.ssdlite.ssdlite320_mobilenet_v3_large(
        weights_backbone = "DEFAULT", num_classes = 2
    )
    model.load_state_dict(torch.load(args['weights'])) 
    model.eval() 
    torch_device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    image_transforms = transforms.Compose([
        # transforms.ColorJitter(
        #     brightness = (0.5,1.5), 
        #     contrast = (0.5,1.5), 
        #     saturation = (0.5,1.5), 
        #     hue = (-0.1, 0.1)
        # ),
        # transforms.Resize((300,300)),
        transforms.ToTensor(),  # Convert the image to a PyTorch tensor
        # transforms.Normalize(
        #     mean=[0.485, 0.456, 0.406], 
        #     std=[0.229, 0.224, 0.225]
        # )
    ])

    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    socket.connect("tcp://localhost:5555")
    socket.setsockopt(zmq.CONFLATE, 1)
    socket.setsockopt(zmq.SUBSCRIBE, b"")

    counter = 0;

    while True:
        counter += 1
        if counter > 1000: 
            counter = 0

        message = socket.recv()
        image = np.asarray(bytearray(message), dtype="uint8") 
        image = cv2.imdecode(image, cv2.IMREAD_COLOR)

        # Keep trying till image exists
        if image is None: continue
        if counter % 4 != 0: continue

        boxes, scores = model_inference(
            image,
            torch_device,
            image_transforms,
            model
        )

        # # Keep trying till boxes exist
        if len(boxes) >= 1:
            p1, p2 = display_bbox(image, boxes, scores) 
            cv2.rectangle(image, p1, p2, (0,255,0),2)

        cv2.imshow("Camera", image)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break



if __name__ == "__main__": 
    parser = argparse.ArgumentParser() 
    parser.add_argument('-w','--weights', default = 'weights/', help = 'path to save/load weights') 
    parser.add_argument('-s', '--show', action = 'store_true', help = 'Cv2 displays', default = True) 
    parser.add_argument('-t', '--threshold', default = 0.65, help = 'Model Threshold for box')
    args = vars(parser.parse_args()) 
    run(args)