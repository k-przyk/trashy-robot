#Script to load provided model onto the edge
import numpy as np
import torch 
import os 
import torchvision.models as models 
import argparse 
import onnx 
from onnxsim import simplify 
import blobconverter 
import openvino as ov 
import openvino.runtime as ovr 


def save_onnx(model,path): 
    sample_tensor = torch.randn(1,3,320,320,dtype=torch.float32) 
    model(sample_tensor)
    torch.onnx.export(model,
            sample_tensor, 
                path, 
                export_params=True, 
                opset_version=11,
                do_constant_folding=True,) 


if __name__ == "__main__": 
    #Set up parser
    parser = argparse.ArgumentParser() 
    parser.add_argument('-t', '--threshold', default=0.5, type=float,
                        help='detection threshold')
    parser.add_argument('-w', '--weights', default='weights/', 
            help='load_weights')
    parser.add_argument('-on', '--onnx', default='onnx/', 
            help='onyx save path')
    parser.add_argument('-o', '--output', default='output/', 
            help='blob save path')
    parser.add_argument('-i', '--ir', default='ir/', 
            help='openvion save path')
    args = vars(parser.parse_args())
    model = models.detection.ssdlite.ssdlite320_mobilenet_v3_large(weights_backbone = "DEFAULT", num_classes=2)
    model.load_state_dict(torch.load(args['weights'])) 
    model.eval() 
    onnx_path = os.path.join(args['onnx'],"model.onnx") 
    output_path = os.path.join(args['output'],"model.blob") 
    simplified_onnx_path = os.path.join(args['onnx'],os.path.join("simplified","model.onnx")) 
    ir_model_path = os.path.join(args['ir'],"ir_model.xml")
    ir_bin_path = os.path.join(args['ir'],"ir_model.bin")
    save_onnx(model,onnx_path) 
    onnx_model = onnx.load(onnx_path) 
    model_simplified,check = simplify(onnx_model) 
    onnx.save(model_simplified,simplified_onnx_path) 
   # blobconverter.from_onnx(model=simplified_onnx_path, output_dir='ssd.blob', shaves = 6, use_cache=True, data_type='FP16') 
    ov_model = ov.convert_model(simplified_onnx_path) 
    ov.save_model(ov_model,ir_model_path) 
    
    print(ir_model_path) 
    print(ir_bin_path) 
    shape = (1,3,320,320) 
    core = ovr.Core() 
    edge_model = core.read_model(ir_model_path) 
    compiled_model = core.compile_model(edge_model,"Myriad") 
    infer_request = compiled_model.create_infer_request() 
    # Get input tensor by index
    input_tensor1 = infer_request.get_input_tensor(0)
    print("Input tensor 1: " + str(input_tensor1)) 
    print("Type: " + str(input_tensor1.data.dtype)) 
    assert input_tensor1.data.dtype == np.float32
    results = infer_request.infer()
    output_tensor = infer_request.get_output_tensor(0)
    # Element types, names and layouts are aligned with framework
    print(output_tensor)
    print(type(output_tensor))
    # process output data ...




