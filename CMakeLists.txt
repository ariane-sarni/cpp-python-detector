cmake_minimum_required(VERSION 3.10)
project(CppYolo)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# --- Add Dependencies ---
include(FetchContent)

# Add CPR (C++ Requests)
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.5 # stable tag
)
FetchContent_MakeAvailable(cpr)

# Add nlohmann/json
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)
# --- End Dependencies ---


add_executable(detector main.cpp)

# Link all libraries
target_link_libraries(detector 
    ${OpenCV_LIBS} 
    cpr::cpr 
    nlohmann_json::nlohmann_json
)