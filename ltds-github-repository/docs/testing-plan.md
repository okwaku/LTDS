# Testing Plan

## Functional Tests

| Test | Expected Result |
|---|---|
| Power-on test | ESP32 boots and prints IP address |
| LiDAR startup test | RPLIDAR responds and begins scanning |
| Dashboard load test | Browser loads dashboard at ESP32 IP |
| WebSocket test | Dashboard status changes to connected |
| Point cloud display | Scan points appear on circular canvas |
| Classification test | Moving human-sized cluster is labeled `PERSON` |

## Accuracy Tests

| Test | Method | Expected Result |
|---|---|---|
| Static object detection | Place box/chair at known distances | Cluster appears as `OBJECT` |
| Person detection | Walk through scan field | Moving cluster appears as `PERSON` |
| Wall false positive check | Scan near flat reflective wall | Wall is labeled `WALL` or ignored as non-person |
| Range test | Move target from 1m to 6m | Target remains visible within max range |

## Performance Tests

| Test | Expected Result |
|---|---|
| Continuous runtime | Runs for approximately 2 hours on 5V power bank |
| WebSocket stability | Browser connection remains active for extended test |
| Latency check | Dashboard updates once per scan revolution |
| Payload size check | WebSocket packet remains small enough for stable updates |

## Mechanical Tests

| Test | Expected Result |
|---|---|
| Enclosure fit check | Sensor, ESP32, and battery fit without blocking scan |
| Roll test | Device rolls without internal components shifting |
| Self-righting test | Ballast returns LiDAR to usable orientation |
| Latch/hinge check | Enclosure opens and closes securely |

## Known Prototype Limitations

- Classification is geometry-based, not machine learning based.
- Reflective surfaces may create false positives.
- High scan payloads can increase latency.
- No authentication is enabled in the prototype firmware by default.
