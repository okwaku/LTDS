# LiDAR Threat Detection System (LTDS)

A senior-design prototype for a portable, spherical LiDAR sensing device that provides real-time 360-degree spatial awareness through an ESP32-hosted browser dashboard.

The system is designed around an **RPLIDAR A1M8**, **Arduino Nano ESP32**, **5V USB power bank**, and a **browser-based WiFi/WebSocket dashboard**. It filters raw LiDAR scan data, groups points into clusters, estimates movement, and classifies detected clusters as `PERSON`, `OBJECT`, or `WALL`.

> **Status:** Academic proof-of-concept repository generated from the LTDS final report. The firmware and documentation are GitHub-ready, but the code should be validated on the actual hardware before operational use.

---

## Key Features

- 360-degree RPLIDAR scanning
- ESP32 local processing
- Real-time WebSocket data stream
- Browser dashboard with HTML5 canvas visualization
- Simple geometric clustering and classification
- Person/object labeling based on cluster width and movement
- No camera or facial recognition data
- Portable 5V battery-powered hardware design
- Documentation for wiring, testing, security, and deployment

---

## Repository Structure

```text
.
├── firmware/
│   ├── include/
│   │   ├── config.example.h
│   │   └── dashboard.h
│   └── src/
│       └── main.cpp
├── dashboard/
│   └── index.html
├── docs/
│   ├── bom.csv
│   ├── deployment.md
│   ├── hardware.md
│   ├── requirements-traceability.md
│   ├── security.md
│   ├── software-architecture.md
│   ├── testing-plan.md
│   ├── user-guide.md
│   └── report/
│       └── LTDS_Final_Report_Complete.pdf
├── samples/
│   └── sample_scan.json
├── tools/
│   ├── README.md
│   └── generate_mock_scan.py
├── cad/
│   └── README.md
├── platformio.ini
├── LICENSE
└── .gitignore
```

---

## Hardware Summary

| Component | Purpose |
|---|---|
| RPLIDAR A1M8 | 360-degree distance scanning |
| Arduino Nano ESP32 | Processing, WiFi, WebSocket server |
| 5V USB power bank | Portable power source |
| PETG spherical enclosure | Protective rollable housing |
| Clear acrylic window | LiDAR scan window |
| Internal cradle | Mounts ESP32, battery, and wiring |
| Ballast weight | Low center of gravity for self-righting behavior |

---

## Firmware Quick Start

### 1. Install PlatformIO

Install [PlatformIO](https://platformio.org/) through VS Code or the PlatformIO CLI.

### 2. Configure WiFi

Copy the example config file:

```bash
cp firmware/include/config.example.h firmware/include/config.h
```

Edit `firmware/include/config.h`.

By default, the example uses access-point mode:

```cpp
#define LTDS_WIFI_MODE_AP 1
#define WIFI_SSID "LTDS-Prototype"
#define WIFI_PASSWORD "ltds123456"
```

For station mode, set:

```cpp
#define LTDS_WIFI_MODE_AP 0
#define WIFI_SSID "your-network"
#define WIFI_PASSWORD "your-password"
```

### 3. Upload

From the repository root:

```bash
pio run -t upload
```

### 4. View Dashboard

Open the ESP32 serial monitor:

```bash
pio device monitor
```

Then open the printed IP address in a browser. The dashboard should automatically connect to:

```text
ws://<device-ip>/ws
```

---

## Data Format

The WebSocket server publishes JSON payloads:

```json
{
  "device": "LTDS",
  "uptime_ms": 123456,
  "points": [
    { "a": 35.2, "d": 1840 }
  ],
  "clusters": [
    {
      "label": "PERSON",
      "x": 1250,
      "y": 320,
      "width": 520,
      "points": 12,
      "moving": true
    }
  ]
}
```

Where:

- `a` is angle in degrees
- `d` is distance in millimeters
- `x`, `y`, and `width` are in millimeters
- `label` is `PERSON`, `OBJECT`, or `WALL`

---

## Safety Notice

This repository is an academic prototype. It is **not certified** for real emergency, tactical, military, law-enforcement, or safety-critical deployment. Before field use, validate laser safety, RF compliance, electrical safety, enclosure durability, battery safety, cybersecurity, and classification performance.

---

## Team

Samuel Franco, Corinthian Bray, Owusu Kwaku, Nolan Hatchell-McNeil, Bryan Barzola  
Stevens Institute of Technology, Department of Electrical and Computer Engineering  
Spring 2026
