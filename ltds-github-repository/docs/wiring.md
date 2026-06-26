# Wiring Reference

## UART

```text
RPLIDAR TX  -> ESP32 GPIO20 RX
RPLIDAR RX  -> ESP32 GPIO21 TX
RPLIDAR GND -> ESP32 GND
RPLIDAR 5V  -> 5V power rail
```

## Power Switch

Wire the SPST power switch in series with the positive 5V output from the USB power bank.

```text
Power bank 5V + -> Switch -> 5V rail
Power bank GND  -> GND rail
```

## Practical Build Notes

- Keep the LiDAR scan plane clear.
- Strain-relieve UART and power wiring.
- Secure the power bank so it cannot shift during rolling.
- Place ballast low in the lower hemisphere to create self-righting behavior.
- Verify LiDAR startup before closing the enclosure.
