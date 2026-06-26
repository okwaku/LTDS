#pragma once

/*
 * LTDS configuration example.
 *
 * Copy this file to firmware/include/config.h and edit the values.
 * firmware/include/config.h is ignored by git so WiFi credentials are not committed.
 */

// 1 = ESP32 creates its own WiFi access point.
// 0 = ESP32 joins an existing WiFi network.
#define LTDS_WIFI_MODE_AP 1

#define WIFI_SSID "LTDS-Prototype"
#define WIFI_PASSWORD "ltds123456"

// Arduino Nano ESP32 UART remap for RPLIDAR A1M8.
#define LIDAR_RX_PIN 20
#define LIDAR_TX_PIN 21
#define LIDAR_BAUD 115200

// Filtering and classification settings.
#define MAX_RANGE_MM 6000
#define CLUSTER_GAP_MM 220
#define MOVEMENT_THRESHOLD_MM 150
#define PERSON_MIN_WIDTH_MM 300
#define PERSON_MAX_WIDTH_MM 1000
#define WALL_WIDTH_MM 1800

// Keep WebSocket payloads smaller by sending every nth scan point.
// Use 1 for full point cloud output.
#define POINT_STRIDE 3
