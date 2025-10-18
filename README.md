# PiCar-X C++ Robotics Project

## Description
This project is a **custom C++ implementation** for the **SunFounder PiCar-X**, designed to provide **low-level motor, steering, and sensor control** using a modular, **object-oriented architecture**.

The current version focuses on **basic movement and sensor integration**, laying a foundation for future upgrades such as **PID control** for precise motor/steering adjustments and **SLAM (Simultaneous Localization and Mapping)** for autonomous navigation.

Unlike most PiCar-X tutorials that rely on Python, this project uses **pure C++**, giving you full control over hardware timing, GPIO access, and modular robotics code.

---

## Features
- Object-Oriented C++ architecture
  - `Motor` class for DC motor control  
  - `Servo` class for steering  
  - `Sensor` class for ultrasonic/IR sensors  
  - `Robot` class to integrate all components  
  - `PIDController` class ready for future speed/steering control
- Basic movement: forward, backward, left/right turns, stop  
- Sensor feedback (ultrasonic/IR) for simple obstacle detection  
- Modular design to easily add PID loops, SLAM, or camera-based vision in the future  
- CMake build system for scalable and organized compilation

---

## Hardware Requirements
- Raspberry Pi (Pi 4, Pi 3B+, or Pi Zero W)  
- SunFounder **PiCar-X Kit** (includes Robot HAT, chassis, motors, servo, ultrasonic/IR sensors)  
- Battery pack (sufficient to power motors + Raspberry Pi)  
- Jumper wires and connectors as needed

---

## Software Requirements
- **C++17** compatible compiler (g++ recommended)  
- **WiringPi** or **pigpio** library for GPIO/PWM access  
- CMake ≥ 3.10  
- Optional future libraries for PID/SLAM:
  - Eigen (matrix operations)
  - OpenCV (camera-based navigation)
  - ROS2 (advanced robotics)

---

## Project Structure
