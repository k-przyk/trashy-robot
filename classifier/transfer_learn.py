import torch 
import torch.nn as nn 
import torchvision.models as models 


#TODO: Write the arg parser 

#Import Mobile Net SSD 
mobile_netv3 = models.detection.ssdlite.ssdlite320_mobilenet_v3_large(weights_backbone = "DEFAULT", num_classes = 1) 
# mobile_netv3 = models.detection.ssdlite.ssdlite320_mobilenet_v3_large(pretrained_backbone = True, num_classes = 1)
# weights = mobile_netv3.weights_backbone
for param in mobile_netv3.parameters(): 
    param.requires_grad = False 
for head_param in mobile_netv3.head.parameters(): 
    head_param.requires_grad = True  
#TODO: What is the format of annotations. Should just be bounding box width + height 


#TODO: Write a Dataloader to load in data + Optimizer? Train model on that. 

