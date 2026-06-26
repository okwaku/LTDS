#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <RPLidar.h>
#include <vector>
#include <algorithm>
#include <math.h>

#if __has_include("config.h")
  #include "config.h"
#else
  #warning "Using default LTDS config. Copy config.example.h to config.h for real deployments."
  #define LTDS_WIFI_MODE_AP 1
  #define WIFI_SSID "LTDS-Prototype"
  #define WIFI_PASSWORD "ltds123456"
  #define LIDAR_RX_PIN 20
  #define LIDAR_TX_PIN 21
  #define LIDAR_BAUD 115200
  #define MAX_RANGE_MM 6000
  #define CLUSTER_GAP_MM 220
  #define MOVEMENT_THRESHOLD_MM 150
  #define PERSON_MIN_WIDTH_MM 300
  #define PERSON_MAX_WIDTH_MM 1000
  #define WALL_WIDTH_MM 1800
  #define POINT_STRIDE 3
#endif

#include "dashboard.h"

struct ScanPoint {
  float angleDeg;
  float distanceMm;
};

struct Cluster {
  String label;
  float xMm;
  float yMm;
  float widthMm;
  int pointCount;
  bool moving;
};

RPLidar lidar;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

std::vector<ScanPoint> scanBuffer;
std::vector<Cluster> previousClusters;

static constexpr size_t MIN_POINTS_PER_SCAN = 24;
static constexpr size_t MAX_SCAN_POINTS = 720;

float degToRad(float deg) {
  return deg * PI / 180.0f;
}

void polarToCartesian(const ScanPoint& p, float& x, float& y) {
  float rad = degToRad(p.angleDeg);
  x = cosf(rad) * p.distanceMm;
  y = sinf(rad) * p.distanceMm;
}

float dist2d(float ax, float ay, float bx, float by) {
  float dx = ax - bx;
  float dy = ay - by;
  return sqrtf(dx * dx + dy * dy);
}

std::vector<ScanPoint> filterPoints(const std::vector<ScanPoint>& input) {
  std::vector<ScanPoint> out;
  out.reserve(input.size());

  for (const auto& p : input) {
    if (p.distanceMm > 0 && p.distanceMm <= MAX_RANGE_MM && p.angleDeg >= 0 && p.angleDeg < 360) {
      out.push_back(p);
    }
  }

  std::sort(out.begin(), out.end(), [](const ScanPoint& a, const ScanPoint& b) {
    return a.angleDeg < b.angleDeg;
  });

  return out;
}

float pointDistanceMm(const ScanPoint& a, const ScanPoint& b) {
  float ax, ay, bx, by;
  polarToCartesian(a, ax, ay);
  polarToCartesian(b, bx, by);
  return dist2d(ax, ay, bx, by);
}

Cluster buildCluster(const std::vector<ScanPoint>& points, size_t start, size_t end) {
  Cluster c;
  c.xMm = 0;
  c.yMm = 0;
  c.widthMm = 0;
  c.pointCount = static_cast<int>(end - start + 1);
  c.moving = false;
  c.label = "OBJECT";

  if (start > end || end >= points.size()) {
    return c;
  }

  float sumX = 0;
  float sumY = 0;
  for (size_t i = start; i <= end; i++) {
    float x, y;
    polarToCartesian(points[i], x, y);
    sumX += x;
    sumY += y;
  }

  c.xMm = sumX / c.pointCount;
  c.yMm = sumY / c.pointCount;

  float sx, sy, ex, ey;
  polarToCartesian(points[start], sx, sy);
  polarToCartesian(points[end], ex, ey);
  c.widthMm = dist2d(sx, sy, ex, ey);

  float nearestPrevious = 999999.0f;
  for (const auto& prev : previousClusters) {
    nearestPrevious = min(nearestPrevious, dist2d(c.xMm, c.yMm, prev.xMm, prev.yMm));
  }
  c.moving = previousClusters.empty() ? false : (nearestPrevious > MOVEMENT_THRESHOLD_MM);

  if (c.moving && c.widthMm >= PERSON_MIN_WIDTH_MM && c.widthMm <= PERSON_MAX_WIDTH_MM) {
    c.label = "PERSON";
  } else if (c.widthMm >= WALL_WIDTH_MM) {
    c.label = "WALL";
  } else {
    c.label = "OBJECT";
  }

  return c;
}

std::vector<Cluster> clusterPoints(const std::vector<ScanPoint>& points) {
  std::vector<Cluster> clusters;
  if (points.empty()) {
    return clusters;
  }

  size_t start = 0;
  for (size_t i = 1; i < points.size(); i++) {
    if (pointDistanceMm(points[i - 1], points[i]) > CLUSTER_GAP_MM) {
      if (i - start >= 3) {
        clusters.push_back(buildCluster(points, start, i - 1));
      }
      start = i;
    }
  }

  if (points.size() - start >= 3) {
    clusters.push_back(buildCluster(points, start, points.size() - 1));
  }

  return clusters;
}

String makeJsonPayload(const std::vector<ScanPoint>& points, const std::vector<Cluster>& clusters) {
  String payload;
  payload.reserve(12000);

  payload += "{\"device\":\"LTDS\",\"uptime_ms\":";
  payload += String(millis());
  payload += ",\"points\":[";

  for (size_t i = 0; i < points.size(); i += max(1, POINT_STRIDE)) {
    if (i > 0) payload += ",";
    payload += "{\"a\":";
    payload += String(points[i].angleDeg, 1);
    payload += ",\"d\":";
    payload += String(static_cast<int>(points[i].distanceMm));
    payload += "}";
  }

  payload += "],\"clusters\":[";
  for (size_t i = 0; i < clusters.size(); i++) {
    const auto& c = clusters[i];
    if (i > 0) payload += ",";
    payload += "{\"label\":\"";
    payload += c.label;
    payload += "\",\"x\":";
    payload += String(static_cast<int>(c.xMm));
    payload += ",\"y\":";
    payload += String(static_cast<int>(c.yMm));
    payload += ",\"width\":";
    payload += String(static_cast<int>(c.widthMm));
    payload += ",\"points\":";
    payload += String(c.pointCount);
    payload += ",\"moving\":";
    payload += c.moving ? "true" : "false";
    payload += "}";
  }

  payload += "]}";
  return payload;
}

void processScan() {
  std::vector<ScanPoint> filtered = filterPoints(scanBuffer);
  std::vector<Cluster> clusters = clusterPoints(filtered);

  String payload = makeJsonPayload(filtered, clusters);
  ws.textAll(payload);

  previousClusters = clusters;

  Serial.printf("Scan: %u raw, %u filtered, %u clusters, payload=%u bytes\n",
                static_cast<unsigned>(scanBuffer.size()),
                static_cast<unsigned>(filtered.size()),
                static_cast<unsigned>(clusters.size()),
                static_cast<unsigned>(payload.length()));
}

void setupWifi() {
#if LTDS_WIFI_MODE_AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("LTDS AP IP address: ");
  Serial.println(WiFi.softAPIP());
#else
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("LTDS station IP address: ");
  Serial.println(WiFi.localIP());
#endif
}

void setupServer() {
  ws.onEvent([](AsyncWebSocket* server,
                AsyncWebSocketClient* client,
                AwsEventType type,
                void* arg,
                uint8_t* data,
                size_t len) {
    if (type == WS_EVT_CONNECT) {
      Serial.printf("Dashboard connected: client %u\n", client->id());
    } else if (type == WS_EVT_DISCONNECT) {
      Serial.printf("Dashboard disconnected: client %u\n", client->id());
    }
  });

  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", DASHBOARD_HTML);
  });

  server.on("/health", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "application/json", "{\"status\":\"ok\",\"device\":\"LTDS\"}");
  });

  server.begin();
}

void setupLidar() {
  Serial1.begin(LIDAR_BAUD, SERIAL_8N1, LIDAR_RX_PIN, LIDAR_TX_PIN);
  lidar.begin(Serial1);

  rplidar_response_device_info_t info;
  if (IS_OK(lidar.getDeviceInfo(info, 1000))) {
    Serial.println("RPLIDAR detected. Starting scan...");
    lidar.startScan();
  } else {
    Serial.println("RPLIDAR not detected. Check wiring and power.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println();
  Serial.println("LTDS firmware booting...");

  scanBuffer.reserve(MAX_SCAN_POINTS);
  setupWifi();
  setupServer();
  setupLidar();
}

void loop() {
  ws.cleanupClients();

  if (IS_OK(lidar.waitPoint())) {
    auto point = lidar.getCurrentPoint();

    if (point.startBit && scanBuffer.size() >= MIN_POINTS_PER_SCAN) {
      processScan();
      scanBuffer.clear();
    }

    if (scanBuffer.size() < MAX_SCAN_POINTS && point.quality > 0) {
      scanBuffer.push_back({point.angle, point.distance});
    }
  } else {
    // If the sensor stops responding, attempt a non-blocking recovery.
    delay(10);
    lidar.startScan();
  }
}
