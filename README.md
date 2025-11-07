# C++ / Python Real-time Object Detection

This project demonstrates a high-performance, multi-language system for real-time object detection. A C++ client captures a webcam feed, compresses frames, and sends them via HTTP to a Python backend. The Python server, built with FastAPI, runs a YOLOv5 model for inference and returns bounding boxes. The C++ client then parses these results and draws them on the live, resizable video feed.

It's designed to be a clean, low-latency example of integrating a high-performance C++ application with a modern Python AI microservice.



---

## Tech Stack

* **Backend (Python):**
    * **FastAPI:** For the high-speed, asynchronous web server.
    * **PyTorch:** For running the machine learning model.
    * **Ultralytics YOLOv5:** For object detection.
    * **Uvicorn:** As the ASGI server to run FastAPI.

* **Client (C++):**
    * **OpenCV:** For webcam capture, image encoding (`imencode`), and drawing (`rectangle`, `putText`).
    * **`cpr` (C++ Requests):** A modern C++ library for making HTTP requests.
    * **`nlohmann/json`:** A header-only C++ library for parsing the JSON response.

* **Build:**
    * **CMake:** Manages the C++ build and uses `FetchContent` to download and link all C++ dependencies automatically.

---

## How to Run

You will need two terminals.

### Prerequisites (CachyOS / Arch Linux)

First, ensure you have the necessary system libraries installed.

```fish
# Install build tools, git, and the C++ OpenCV library
sudo pacman -S git cmake pkg-config opencv python python-pip

### 1. Run the Python Server (Terminal 1)

This terminal will run the FastAPI server that performs the AI inference.

```fish
# 1. Go to the inference directory
cd inference

# 2. Create and activate a Python virtual environment
python -m venv venv
source venv/bin/activate.fish

# 3. Install required Python packages
pip install -r requirements.txt

# 4. Run the server
# (This will download the YOLOv5 model on its first run)
uvicorn server:app --host 127.0.0.1 --port 8000

### 2. Run the C++ Client (Terminal 2)

This terminal will build and run the C++ application that captures your webcam and displays the results.

```fish
# 1. From the project root, create a build directory
mkdir build
cd build

# 2. Configure the project with CMake
# (This will download cpr and nlohmann/json)
cmake ..

# 3. Build the project (using all your CPU cores)
make -j(nproc)

# 4. Run the detector!
./detector

A window will pop up showing your webcam feed with real-time bounding boxes. You can resize this window with your mouse.
