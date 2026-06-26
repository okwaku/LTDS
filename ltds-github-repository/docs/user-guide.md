# User Guide

## Basic Operation

1. Charge the 5V USB power bank.
2. Connect the RPLIDAR and ESP32 wiring.
3. Turn on the LTDS device.
4. Wait for the ESP32 to create or join WiFi.
5. Connect a phone, tablet, or laptop to the LTDS network.
6. Open the ESP32 IP address in a browser.
7. Watch the live point cloud and cluster labels.

## Dashboard Meaning

| Label | Meaning |
|---|---|
| `PERSON` | Moving cluster with human-scale width |
| `OBJECT` | Static or non-human-sized object |
| `WALL` | Large wide cluster likely caused by a wall or surface |

## Recommended Field Workflow

1. Power on the device before deployment.
2. Confirm dashboard connection.
3. Roll or place the device into the target room.
4. Let the scan stabilize.
5. Use the dashboard to understand obstacles and possible human presence.
6. Retrieve and recharge the device after operation.

## Important Warning

This prototype should not be used as the only decision-making tool in emergency environments. It is a proof-of-concept and requires further validation before field deployment.
