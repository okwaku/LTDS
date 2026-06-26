#!/usr/bin/env python3
import json
import math
import random
import time

def make_point(angle, distance):
    return {"a": round(angle, 1), "d": int(distance)}

points = []

# Room-like wall ring with gaps.
for angle in range(0, 360, 5):
    dist = 4200 + random.uniform(-80, 80)
    points.append(make_point(angle, dist))

# Object cluster.
for angle in [38, 40, 42, 44, 46, 48]:
    points.append(make_point(angle, 1800 + random.uniform(-30, 30)))

# Person cluster.
for angle in [205, 207, 209, 211, 213, 215, 217]:
    points.append(make_point(angle, 2500 + random.uniform(-50, 50)))

payload = {
    "device": "LTDS",
    "uptime_ms": int(time.time() * 1000),
    "points": sorted(points, key=lambda p: p["a"]),
    "clusters": [
        {"label": "OBJECT", "x": 1400, "y": 1200, "width": 480, "points": 6, "moving": False},
        {"label": "PERSON", "x": -2200, "y": -900, "width": 560, "points": 7, "moving": True}
    ]
}

print(json.dumps(payload, indent=2))
