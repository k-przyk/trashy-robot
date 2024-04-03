import torch 
import torchvision.models as models 
from torchvision.models.detection import SSDLite320_MobileNet_V3_Large_Weights
from torchvision.datasets.vision import VisionDataset 
import torchvision.transforms as transforms 
import argparse 
import os 
import cv2
import numpy as np 
from torch.utils.data import DataLoader
import collections
from xml.etree.ElementTree import Element as ET_Element
from xml.etree.ElementTree import parse as ET_parse
from typing import Any, Callable, Dict, List, Optional, Tuple
from PIL import Image

def display_bbox(dataset): 
    for data in dataset: 
        # Convert the PIL.Image.Image to a NumPy array
        image, target = data 
        print(image.shape) 
        image_array = np.array(image)
        image_array = np.transpose(image_array, (1,2,0)) 
        print(image_array.shape) 
        image_array = cv2.cvtColor(image_array, cv2.COLOR_RGB2BGR)
        print("Display Target: " + str(target)) 
        for o in target["boxes"]: 
            xmin = o[0].item() 
            ymin = o[1].item()
            xmax = o[2].item() 
            ymax = o[3].item()
            pt1 = (xmin,ymin) 
            pt2 = (xmax,ymax)  
            cv2.rectangle(image_array, pt1, pt2, (0,255,0),2) 
        cv2.imshow('Display window', image_array)
        cv2.waitKey(0)  
        cv2.destroyAllWindows()

def my_collate(batch):
    data = [i[0] for i in batch] 
    target = [i[1] for i in batch]
    return data,target 

# TODO: Proper way is to write my own Dataset properly that maybe contains a VOC dataset? 
class TrashDataset(VisionDataset):
    def __init__(
        self,
        root: str,
        image_set: str = "train",
        transform: Optional[Callable] = None,
        target_transform: Optional[Callable] = None,
        transforms: Optional[Callable] = None,
    ):
        super().__init__(root, transforms, transform, target_transform)
        voc_root = os.path.join(self.root,os.path.join("VOCdevkit", "VOC2012"))
        splits_dir = os.path.join(voc_root, "ImageSets", "Main")
        split_f = os.path.join(splits_dir, image_set.rstrip("\n") + ".txt")
        with open(os.path.join(split_f)) as f:
            file_names = [x.strip() for x in f.readlines()]
        image_dir = os.path.join(voc_root, "JPEGImages")
        self.images = [os.path.join(image_dir, x + ".jpeg") for x in file_names]
        target_dir = os.path.join(voc_root, "Annotations")
        self.targets = [os.path.join(target_dir, x + ".xml") for x in file_names]
        assert len(self.images) == len(self.targets)
    def __len__(self) -> int:
        return len(self.images)
    @property
    def annotations(self) -> List[str]:
        return self.targets
    #Modified so it returns the acutal target/label format needed for training 
    def __getitem__(self, index: int) -> Tuple[Any, Any]:
        img = Image.open(self.images[index]).convert("RGB")
        target = self.parse_voc_xml(ET_parse(self.annotations[index]).getroot())
        objs = target["annotation"]["object"] 
        #Temp just set it to 0? 
        labels = [0] * len(objs) 
        boxes = [[int(bbox["bndbox"]["xmin"]), int(bbox["bndbox"]["ymin"]), int(bbox["bndbox"]["xmax"]), int(bbox["bndbox"]["ymax"])] for bbox in objs]
        target = {"boxes": torch.tensor(boxes), "labels": torch.tensor(labels)}
        if self.transforms is not None:
            img, target = self.transforms(img, target)
        return img, target

    @staticmethod
    def parse_voc_xml(node: ET_Element) -> Dict[str, Any]:
        voc_dict: Dict[str, Any] = {}
        children = list(node)
        if children:
            def_dic: Dict[str, Any] = collections.defaultdict(list)
            for dc in map(TrashDataset.parse_voc_xml, children):
                for ind, v in dc.items():
                    def_dic[ind].append(v)
            if node.tag == "annotation":
                def_dic["object"] = [def_dic["object"]]
            voc_dict = {node.tag: {ind: v[0] if len(v) == 1 else v for ind, v in def_dic.items()}}
        if node.text:
            text = node.text.strip()
            if not children:
                voc_dict[node.tag] = text
        return voc_dict

#NOTE: Made a slight modification to accept jpeg images in voc.py under 
#opt/homebrew/lib/python3.11/site-packages/torchvision/datasets/voc.py 
#Import Mobile Net SSD 
def unnormalize(tensor, mean, std):
    """
    Unnormalize a tensor image with mean and standard deviation.
    Input tensor should be of shape CxHxW.
    mean and std are sequences of means and standard deviations per channel.
    """
    # Duplicate the mean and std to match the tensor's shape
    mean = torch.as_tensor(mean, dtype=tensor.dtype, device=tensor.device).view(-1, 1, 1)
    std = torch.as_tensor(std, dtype=tensor.dtype, device=tensor.device).view(-1, 1, 1)

    # Apply the unnormalize formula
    unnormalized_tensor = tensor * std + mean

    return unnormalized_tensor
def display_bbox_eval(image,boxes):
    image = np.transpose(image, (1,2,0))
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

    # image = np.zeros((540, 960, 3), dtype=np.uint8)
    image_255 = np.clip(image * 255, 0, 255).astype(np.uint8)
    # print(image_255)
    # print("Shape: " + str(image_255.shape))
    # print("Boxes: " + str(boxes)) 
    for bbox in boxes:
        print("BBox: " + str(bbox))
        xmin = bbox[0].item()
        ymin = bbox[1].item()
        xmax = bbox[2].item()
        ymax = bbox[3].item()
        pt1 = (int(xmin),int(ymin))
        pt2 = (int(xmax),int(ymax))
        print("Pt1: " + str(pt1))
        print("Pt2: " + str(pt2))
        cv2.rectangle(image_255, pt1, pt2, (0,255,0),2)
    cv2.imshow("Eval", image_255)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


def eval_model(model, dataloader):
    for images, targets in dataloader: 
        outputs = model(images, targets)
        # print("Outputs: " + str(outputs))
        scores = outputs[0]['scores']
        boxes = outputs[0]['boxes']
        labels = outputs[0]['labels'] 
        # Filter scores greater than 0.87
        # high_score_indices = scores > 0.90
        # Select boxes with scores above 0.87
        high_score_boxes = boxes[0]
        mean = [0.485, 0.456, 0.406]
        std = [0.229, 0.224, 0.225]
        unnormalized_image = unnormalize(images[0],mean,std)
        display_bbox_eval(unnormalized_image.numpy(),[high_score_boxes])
        print("Confidence: " + str(scores[0]))  
        print("Label: " + str(labels[0])) 
        



        # print(images) 
        # print(targets)
        # outputs = model(images, targets)
        # print("Outputs: " + str(outputs))


    
def train_model(model,dataloader,optimizer,num_epochs): 
    for epoch in range(0,num_epochs): 
        for images, targets in dataloader:
            outputs = model(images, targets) 
            losses = sum(loss for loss in outputs.values())
            optimizer.zero_grad()
            losses.backward()
            optimizer.step()
        print(f"Epoch {epoch+1}, Loss: {losses.item()}")

if __name__ == "__main__": 
    #Set up parser
    parser = argparse.ArgumentParser() 
    parser.add_argument('-i', '--input-path', default='rgb_data/', 
                    help='path to input video')
    parser.add_argument('-t', '--threshold', default=0.5, type=float,
                        help='detection threshold')
    parser.add_argument('-c', '--checkpoint', default='checkpoint/', 
                help='path to save checkpoint folder')
    parser.add_argument('-w', '--weights', default='weights/', 
            help='path to save/load weights')
    parser.add_argument('-tr', '--train', action='store_true', help='Flag to train', default = False)
    args = vars(parser.parse_args())
    mobile_netv3 = models.detection.ssdlite.ssdlite320_mobilenet_v3_large(weights_backbone = "DEFAULT", num_classes = 2) 
    # mobile_netv3 = models.detection.ssdlite.ssdlite320_mobilenet_v3_large(weights=SSDLite320_MobileNet_V3_Large_Weights.DEFAULT) 


    #Use transforms
    transforms = transforms.Compose([
        # transforms.Resize((350, 350)),  # Resize the image to 320x320
        transforms.ToTensor(),  # Convert the image to a PyTorch tensor
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])  
    ])

    for param in mobile_netv3.parameters(): 
        param.requires_grad = False 
    for head_param in mobile_netv3.head.parameters(): 
        head_param.requires_grad = True  
    train_flag = args['train'] 
    path = args['input_path'] 
    save = args['weights'] 
    data_path = os.path.join(os.getcwd(), path)
    weights_path = os.path.join(os.path.join(os.getcwd(),save),"trash_weights.pth") 
    num_epochs = 10 
    batch_size = 64
    lr = 1e-2
    if train_flag:
        dataset = TrashDataset(root = data_path, image_set='train', transform = transforms)
        train_dataloader = DataLoader(dataset, batch_size = batch_size, shuffle = True, num_workers = 2, collate_fn = my_collate)
        optimizer = torch.optim.SGD(mobile_netv3.parameters(), lr=lr, momentum=0.9, weight_decay=0.0005)
        train_model(mobile_netv3,train_dataloader, optimizer,num_epochs) 
        torch.save(mobile_netv3.state_dict(),weights_path)
        print("Saved Weights in: " + str(weights_path))
    else:
        dataset = TrashDataset(root = data_path, image_set='val', transform = transforms)
        eval_dataloader = DataLoader(dataset, batch_size = 3, shuffle = True, num_workers = 1, collate_fn = my_collate)
        mobile_netv3.load_state_dict(torch.load(os.path.join(weights_path,weights_path)))
        mobile_netv3.eval()
        eval_model(mobile_netv3,eval_dataloader)
        




    