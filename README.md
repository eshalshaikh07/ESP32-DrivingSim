# ESP32 Steering and Control System Using Encoder 🚗

## Overview 🌐

This project involves controlling a virtual vehicle's steering and acceleration/braking through BLE (Bluetooth Low Energy) using the **ESP32** microcontroller. The system includes:
- **Accelerator** and **Brake** buttons to simulate vehicle controls.
- **Rotary Encoder** to simulate the steering control.

The ESP32 communicates with a device (e.g., a computer or mobile phone) via BLE, sending key presses (arrow keys) corresponding to the accelerator, brake, and steering control. The system can be used for virtual vehicle control, such as in a racing simulator or other applications requiring precise control input.

## Components Used ⚙️

1. **ESP32 Board** 🌐
2. **BLE Keyboard Library** (`BleKeyboard.h`) 📚
3. **Rotary Encoder** (for steering control) 🔄
4. **Push Buttons** (for accelerator and brake controls) 🟢🔴

## Hardware Setup 🔌

### Wiring Diagram 🧑‍🔧:
- **Accelerator Button** 🟢: Connect to GPIO pin **21** (Press = LOW)
- **Brake Button** 🔴: Connect to GPIO pin **22** (Press = LOW)
- **Rotary Encoder** 🔄:
  - **CLK Pin**: Connect to GPIO pin **32**
  - **DT Pin**: Connect to GPIO pin **33**
  - **SW Button** (Optional): Connect to GPIO pin **25**

### Important Libraries 📚

```cpp
#include <BleKeyboard.h>  // Import the BLE Keyboard library to emulate a keyboard
