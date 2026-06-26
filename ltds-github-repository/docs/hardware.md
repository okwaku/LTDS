# Hardware Guide

## Prototype Hardware

The LTDS prototype uses:

- RPLIDAR A1M8 rotating 2D LiDAR sensor
- Arduino Nano ESP32
- 5V USB power bank
- PETG 6-inch spherical enclosure
- Clear acrylic LiDAR window
- Internal cradle
- Power switch
- Ballast weight for self-righting behavior

## Electrical Connections

| Signal | RPLIDAR A1M8 | Arduino Nano ESP32 |
|---|---:|---:|
| UART TX | TX | GPIO20 / RX |
| UART RX | RX | GPIO21 / TX |
| Power | 5V | 5V USB power rail |
| Ground | GND | GND |

The firmware uses `Serial1` at `115200` baud.

## Power

The final report describes a 5V USB power bank rated for approximately 2A output. The estimated full-load current is approximately:

- RPLIDAR A1M8: ~500 mA
- Arduino Nano ESP32 with active WiFi: ~240 mA
- Total: ~750 mA

This fits within a 2A USB power source and supports about 2 hours of continuous operation in the prototype.

## Mechanical Notes

The enclosure is a two-piece 6-inch spherical PETG housing with a clear acrylic equatorial sensor window. The internal cradle should secure the LiDAR, ESP32, wiring, and power bank while keeping wiring out of the scan plane.
