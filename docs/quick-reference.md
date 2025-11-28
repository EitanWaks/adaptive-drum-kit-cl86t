# Quick Reference Card

## Serial Commands

```
S/s  - Stop motor
H/h  - Home (move to home position)
0    - Zeroing home (set current position as home)
+    - Increase speed (+100 steps/sec)
-    - Decrease speed (-100 steps/sec)
?    - Show status
N/n  - Nudge (move 1 step)
M/m  - Map (set angle for trigger, prompts for degrees, no movement)
T/t  - Trigger (rotate stored angle CW then return CCW)
I/i  - Inverse (reverse clockwise/counterclockwise)
E/e  - Toggle enable/disable
```

## Pin Connections

### Arduino to CL86T
```
Pin 2  → PUL+  (Step)
Pin 3  → DIR+  (Direction)
Pin 4  → ENA+  (Enable)
GND    → PUL-, DIR-, ENA-  (Ground)
```

### Power Supply to CL86T
```
LE-350-60 +V (any one) → CL86T AC (first terminal)
LE-350-60 -V (any one) → CL86T AC (second terminal)
```

**Note:** CL86T AC terminals accept DC input (no polarity requirement for DC)

### Motor to CL86T (✅ Verified Configuration)
```
Black  → A+  (Phase A positive)
Green  → A-  (Phase A negative)
Red    → B+  (Phase B positive)
Blue   → B-  (Phase B negative)
```

## Default Settings

- **Steps per revolution**: 200
- **Default speed**: 500 steps/sec
- **Speed range**: 100-2000 steps/sec
- **Acceleration**: 1000 steps/sec²
- **Serial baud rate**: 115200

## Driver Settings

### DIP Switches (SW1-SW8)
- **SW1-SW4**: All ON (200 pulse/rev, full step)
- **SW5**: ON (Clockwise) ⚠️ **CORRECTED:** ON = CW, OFF = CCW
- **SW6**: OFF (Closed Loop)
- **SW7**: OFF (Pul/Dir)
- **SW8**: OFF (Brake Output)

### Current Setting (S1 Rotary Switch)
- **Recommended for 34E1K-120**: Position **A** (5.7A RMS, 8.0A Peak)
- **Motor rated**: 6.0A, driver max: 5.7A RMS (95% of rating - safe)
- **Alternative**: Position 4 (5.0A RMS) if motor runs too hot
- **See**: [Current Settings Guide](current-settings.md) for details

## Common Issues

| Problem | Solution |
|---------|----------|
| Motor doesn't move | Check power, enable, wiring |
| Motor vibrates | Swap A+/A- or B+/B- |
| Wrong direction | Change DIR_INVERT in pins.h |
| Arduino not responding | Check USB, COM port, baud rate |

## Configuration Files

- **Pin assignments**: `include/pins.h`
- **Main code**: `src/main.cpp`
- **PlatformIO config**: `platformio.ini`

## Useful Commands

### Test Motor
```
?    # Check status
n    # Nudge (1 step)
m    # Map angle, then enter: 90
t    # Trigger (rotate and return)
```

### Adjust Speed
```
+    # Faster
-    # Slower
?    # Check current speed
```

### Position Control
```
H    # Go home
0    # Set current position as home
n    # Nudge (1 step)
m    # Map angle (stores for trigger)
t    # Trigger (uses mapped angle)
```

## Heat Dissipation

- **Mount CL86T vertically** (heat sink fins vertical) for best cooling
- **Maximum temperature:** 40°C (109°F) - monitor during operation
- **Add cooling fan** if temperature exceeds 35°C or if running at max current
- **Spacing:** If multiple drivers, keep 30mm (12 inches) between them
- **Clearance:** At least 50mm around driver for airflow

## Important Setup Notes

- **5V/24V Switch:** ⚠️ **Must set rotating switch to 5V** (factory default is 24V)
- **LED Indicators:** GREEN = power ON, RED = alarm (should be OFF normally)
- **Encoder Power:** CL86T VCC supplies 5V to encoder (don't connect external 5V)

## Safety Reminders

⚠️ **Always disconnect power before wiring**
⚠️ **Double-check connections before powering on**
⚠️ **60V DC can be dangerous - handle with care**
⚠️ **Ensure proper grounding**
⚠️ **Monitor driver temperature - keep below 40°C (109°F)**
⚠️ **Set 5V/24V switch to 5V for Arduino compatibility**

