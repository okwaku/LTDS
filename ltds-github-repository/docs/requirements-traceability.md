# Requirements Traceability

| Requirement | Implementation / Repository Location |
|---|---|
| Real-time object and threat detection | `firmware/src/main.cpp` clustering and classification pipeline |
| Wireless data transmission | ESP32 WiFi and WebSocket server in `firmware/src/main.cpp` |
| User-friendly dashboard | `dashboard/index.html` and `firmware/include/dashboard.h` |
| Autonomous portable operation | `docs/hardware.md`, `docs/deployment.md` |
| RPLIDAR A1M8 integration | `firmware/src/main.cpp`, UART settings in `config.example.h` |
| ESP32 local processing | Scan filter, clustering, movement, and classification logic |
| Browser display | HTML5 canvas dashboard |
| Maintainable software | Modular documentation and commented firmware |
| Privacy-safe operation | No image capture and no default persistent data storage |
| Durability and manufacturability | `docs/hardware.md`, `cad/README.md`, `docs/bom.csv` |
