cmake_minimum_required(VERSION 3.10)
project(TDCar VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV package
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Include directories
include_directories(include)

# Library for shared code
add_library(tdcar_lib
    src/motor.cpp
    src/robot_hat_pwm.cpp
    src/camera.cpp
)

# Link libraries
target_link_libraries(tdcar_lib wiringPi pthread m ${OpenCV_LIBS})

# Main executable
add_executable(picar_drive src/main.cpp)
target_link_libraries(picar_drive tdcar_lib)

# Test executable
add_executable(motor_class_test test/motor_class_test.cpp)
target_link_libraries(motor_class_test tdcar_lib)

# Camera test executable
add_executable(camera_test src/camera_test.cpp)
target_link_libraries(camera_test tdcar_lib ${OpenCV_LIBS})

# Install targets
install(TARGETS picar_drive camera_test DESTINATION bin)
