# Security Notes

The current prototype is designed for demonstration and lab use.

## Current Prototype Risks

- WiFi credentials are compile-time settings.
- Prototype dashboard does not require authentication.
- WebSocket messages are not encrypted.
- Access is limited only by local WiFi/network access.

## Recommended Improvements

- Use WPA2/WPA3 for the WiFi network.
- Add token-based WebSocket authentication.
- Use HTTPS/WSS for deployed versions.
- Rotate field passwords.
- Disable dashboard access when not in use.
- Avoid persistent scan logging unless required.
- If logging is added, define retention and privacy rules.

## Privacy Position

The LTDS concept avoids camera images and facial recognition. It only displays LiDAR point-cloud-derived spatial data. This reduces privacy exposure compared with camera-based systems, but scan data should still be treated as operationally sensitive.
