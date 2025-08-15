import torch
from PIL import Image
import io
import uvicorn
from fastapi import FastAPI, File, UploadFile
from fastapi.responses import JSONResponse
import numpy as np
import cv2

# basic test script for now. will become server.

# load model - using yolov5
# todo: make this model name configurable
model = torch.hub.load('ultralytics/yolov5', 'yolov5s', pretrained=True)

def run_inference_on_image(img_path):
    print(f"Running test inference on {img_path}")
    img = Image.open(img_path)
    results = model(img)
    
    # show results
    results.show()
    
    # or print them
    print("Detections:")
    print(results.pandas().xyxy[0])

if __name__ == "__main__":
    # basic test to make sure model works
    # YOU MUST ADD A 'test_image.jpg' file to this directory
    print("running basic test...")
    # run_inference_on_image('test_image.jpg')
    print("test complete. server.py is ready to be built out")