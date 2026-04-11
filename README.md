# Driver Monitoring System

A cyber-physical driver drowsiness detection system built for EECS 3216. A computer vision model running on a host laptop uses OpenCV and Dlib to monitor the driver's Eye Aspect Ratio (EAR) in real time, sending a CAN signal over a USB-CAN adapter when drowsiness is detected. An Arduino Uno R4 Minima receives the signal via an MCP2515 CAN controller using a hardware interrupt, and responds by repeatedly stopping and starting the vehicle's DC motors to alert the driver.

## Requirements
**Hardware:** Arduino Uno R4 Minima, MCP2515 CAN module, CANable USB-CAN adapter, L298N motor driver, DC motors  
**Firmware:** PlatformIO, [arduino-mcp2515](https://github.com/autowp/arduino-mcp2515)  
**Software:** Python 3, OpenCV, Dlib, python-can, SocketCAN (Linux)

## Usage
1. Flash the Arduino firmware via PlatformIO.
2. Connect the CANable and bring up the CAN interface: `sudo ip link set can0 up type can bitrate 500000`
3. Run `calibrate.py` to generate a personalised EAR threshold for the driver.
4. Run `Finished.py` to start monitoring.