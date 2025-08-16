import torch
from PIL import Image
import io
import uvicorn
from fastapi import FastAPI, File, UploadFile
from fastapi.responses import JSONResponse
import numpy as np
import cv2

app = FastAPI(title="YOLOv5 Inference Server")

# load model on startup
print("Loading YOLOv5 model...")
model = torch.hub.load('ultralytics/yolov5', 'yolov5s', pretrained=True)
model.eval() # set to evaluation mode
print("Model loaded.")

@app.post("/detect/")
async def detect_objects(file: UploadFile = File(...)):
    # read image bytes
    contents = await file.read()
    
    # convert bytes to PIL image
    img = Image.open(io.BytesIO(contents))
    
    # run inference
    results = model(img)
    
    # format results as json
    # results.pandas().xyxy[0] is a df. convert to json
    detections = results.pandas().xyxy[0].to_dict(orient='records')
    
    # detections format:
    # [
    #   {'xmin': 1.0, 'ymin': 2.0, 'xmax': 3.0, 'ymax': 4.0, 'confidence': 0.9, 'class': 0, 'name': 'person'},
    #   ...
    # ]
    
    return JSONResponse(content=detections)

if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8000)