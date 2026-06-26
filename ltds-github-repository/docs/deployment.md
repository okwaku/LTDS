# Deployment Notes

## Access Point Mode

Access point mode is recommended for demonstrations and field tests without infrastructure.

```cpp
#define LTDS_WIFI_MODE_AP 1
#define WIFI_SSID "LTDS-Prototype"
#define WIFI_PASSWORD "ltds123456"
```

Then connect your laptop/phone to the LTDS WiFi network and open the IP printed in the serial monitor.

## Station Mode

Station mode is useful in a lab where the ESP32 and dashboard client are on the same router.

```cpp
#define LTDS_WIFI_MODE_AP 0
#define WIFI_SSID "your-router"
#define WIFI_PASSWORD "your-password"
```

## Serial Monitor

Use:

```bash
pio device monitor
```

Look for:

```text
LTDS AP IP address: 192.168.4.1
RPLIDAR detected. Starting scan...
```

## Troubleshooting

| Problem | Possible Cause | Fix |
|---|---|---|
| No dashboard | Wrong WiFi network | Connect to LTDS AP or same router |
| WebSocket disconnected | Device IP changed | Reload correct IP address |
| No points | LiDAR wiring or power issue | Check 5V, GND, TX/RX pins |
| Sensor not detected | UART pins swapped | Verify TX -> RX and RX -> TX |
| Laggy dashboard | Payload too large | Increase `POINT_STRIDE` |
