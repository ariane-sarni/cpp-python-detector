# C++ / Python Real-time Detection

A high-performance C++ app that captures a webcam feed, sends frames to a Python/YOLO
server for inference, and draws the resulting bounding boxes in real-time.

## Tech
- **C++:** OpenCV (webcam, drawing), CPR (http client), nlohmann/json (json parsing)
- **Python:** FastAPI (web server), PyTorch/YOLOv5 (inference)
- **Glue:** HTTP (sending jpeg-encoded frames from C++ to Python)

## How to Run

You need two terminals.

### 1. Run the Python Server
First, set up the Python environment:
```bash
cd inference
python -m venv venv
source venv/bin/activate.fish  # Use .fish for fish shell
pip install -r requirements.txt