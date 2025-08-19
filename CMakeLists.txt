cmake_minimum_required(VERSION 3.13)
project(CppYolo)

find_package(OpenCV REQUIRED COMPONENTS core highgui videoio imgcodecs)
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

# Add nlohmann/json (header only)
# We will download it, but not "build" it.
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_GetProperties(nlohmann_json)
if(NOT nlohmann_json_POPULATED)
  FetchContent_Populate(nlohmann_json)
endif()
# --- End Dependencies ---


add_executable(detector main.cpp)

# Link all libraries
target_link_libraries(detector 
    ${OpenCV_LIBS} 
    cpr::cpr 
    # nlohmann_json is header-only, no target to link
)

# Add the include directory for nlohmann_json
target_include_directories(detector PRIVATE 
    ${nlohmann_json_SOURCE_DIR}/include
)