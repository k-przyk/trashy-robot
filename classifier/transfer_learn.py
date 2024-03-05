import torch 
import torch.nn as nn 
import torchvision.models as models 
import torchvision.models.detection.ssd as ssd 

#Import Mobile Net SSD 
mobile_netv3 = models.get_model("ssdlite320_mobilenet_v3_large", weights_backbone = "DEFAULT", num_classes = 1) 
# print("Mobile_netV3_weights: " + str(mobile_netv3_weights))
# in_channels = mobile_netv3.head.classification_head.in_channels 
# num_anchors = mobile_netv3.head.classification_head.num_anchors 
# num_classes = 2 
# mobile_netv3.head.classification_head = ssd.SSDClassificationHead(in_channels, num_anchors, num_classes) 
# mobile_netv3.backbone.features[-1].out_channels = 2
# print("Param List: ") 
# for k,v in mobile_netv3.named_parameters(): 
#     print(k)
#Freeze the model 
for param in mobile_netv3.parameters(): 
    param.requires_grad = False 
for head_param in mobile_netv3.head.parameters(): 
    head_param.requires_grad = True  
    # print("Head Param: " + str(head_param.requires_grad)) 
# print("Backbone:") 
# print(mobile_netv3.backbone)
# print("Regression Head:") 
# print(mobile_netv3.head.regression_head)
# print("Classification  Head:") 
# print(mobile_netv3.head.classification_head)
mobile_netv3.eval() 

