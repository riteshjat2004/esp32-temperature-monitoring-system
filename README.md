# ESP32 IoT Temperature Monitoring & Control System

An IoT-based embedded system developed using ESP32 and DHT22 to monitor temperature and humidity in real time. Sensor data is uploaded to ThingSpeak over Wi-Fi, while a threshold-based control mechanism automatically drives an actuator (fan/LED).

## Features

- Real-time temperature and humidity monitoring
- Wi-Fi connectivity using ESP32
- Cloud data logging via ThingSpeak
- Automatic fan/LED control using configurable temperature threshold
- Sensor data validation
- Wi-Fi reconnection logic
- Real-time dashboard visualization

## Hardware

- ESP32 Development Board
- DHT22 Sensor
- Relay Module
- Fan / LED
- Breadboard
- Jumper Wires

## Software

- Arduino IDE
- Embedded C
- ThingSpeak
- WiFi Library
- DHT Library

## Project Workflow

DHT22
↓
ESP32
↓
Temperature Processing
↓
Threshold Check
↓
ThingSpeak Cloud
↓
Dashboard

## Future Improvements

- MQTT Support
- OTA Firmware Updates
- Multiple Sensor Nodes
- Mobile Dashboard
