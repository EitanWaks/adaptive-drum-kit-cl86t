# Adaptive Drum Kit - CL86T Motor Control

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Arduino-blue.svg)](https://platformio.org/)
[![Arduino](https://img.shields.io/badge/Arduino-Uno%20R3-green.svg)](https://www.arduino.cc/)

An implementation for controlling the 34E1K-120 stepper motor using the CL86T closed-loop driver.

## 🎯 Project Overview

This is an implementation designed for the hardware stack:
- **Stepper Motor**: 34E1K-120 (NEMA 34, 200 steps/rev)
- **Driver**: CL86T (Closed-loop stepper driver)
- **Power Supply**: LE-350-60 (60V DC, 5.8A)
- **Microcontroller**: Arduino UNO R3

## ✨ Features

- **Simple Serial Control**: Control motor via serial commands from computer
- **AccelStepper Library**: Smooth acceleration and deceleration
- **Flexible Commands**: Move forward, backward, rotate by steps or degrees
- **Speed Control**: Adjustable speed with real-time changes
- **Position Control**: Move to specific positions or home
- **Enable Control**: Enable/disable motor as needed
- **Well Documented**: Comprehensive wiring diagrams and setup guides

## 🚀 Quick Start

### Prerequisites

- Arduino UNO R3
- PlatformIO IDE (or VS Code with PlatformIO extension)
- All hardware components (motor, driver, power supply)
- USB cable for Arduino

### Installation

1. **Clone or navigate to project directory**
   ```bash
   cd code/adaptive-drum-kit-cl86t
   ```

2. **Install PlatformIO dependencies**
   ```bash
   pio lib install
   ```

3. **Connect hardware** (see [Wiring Diagram](docs/wiring-diagram.md))
   - Connect power supply to CL86T driver
   - Connect motor to CL86T driver
   - Connect Arduino to CL86T driver (pins 2, 3, 4, GND)

4. **Upload firmware**
   ```bash
   pio run --target upload
   ```

5. **Open serial monitor**
   ```bash
   pio device monitor
   ```
   Or use PlatformIO Serial Monitor (baud rate: 115200)

### First Test

1. Open serial monitor (115200 baud)
2. Type `?` and press Enter to see status
3. Type `n` to nudge (move 1 step)
4. Type `m` then enter `90` to map 90 degrees
5. Type `t` to trigger (rotate and return)

## 📖 Serial Commands

| Command    | Description                                                    | Example       |
| ---------- | -------------------------------------------------------------- | ------------- |
| `S` or `s` | Stop motor immediately                                         | `S`           |
| `H` or `h` | Home - Move to home position (0)                               | `H`           |
| `0`        | Zeroing home - Set current position as home                    | `0`           |
| `+`        | Increase speed by 100 steps/sec                                | `+`           |
| `-`        | Decrease speed by 100 steps/sec                                | `-`           |
| `?`        | Show current status                                            | `?`           |
| `N` or `n` | Nudge - Move 1 step                                            | `n`           |
| `M` or `m` | Map - Set angle for trigger (prompts for degrees, no movement) | `m` then `90` |
| `T` or `t` | Trigger - Rotate stored angle CW then return CCW               | `t`           |
| `I` or `i` | Inverse - Reverse clockwise/counterclockwise                   | `i`           |
| `E` or `e` | Toggle motor enable/disable                                    | `E`           |
| Any other  | Show help menu                                                 | `help`        |

### Command Examples

```
?          # Show status
n          # Nudge (move 1 step)
m          # Map angle, then enter: 90 (stores 90 degrees)
t          # Trigger (rotates stored angle and returns)
h          # Return to home position
0          # Set current position as home
i          # Reverse direction
+          # Increase speed
-          # Decrease speed
s          # Stop immediately
```

### Important Notes

- **Map Command (`m`)**: Stores an angle for use with trigger. Does NOT move the motor.
- **Trigger Command (`t`)**: Uses the angle set by map command. Rotates forward then automatically returns.
- **Safety**: Trigger command ignores new requests while a trigger is in progress to prevent position creep.
- **Default Angle**: No default angle - must use `m` command first before using `t`.

## 🔌 Hardware Connections

### Quick Reference

| Arduino Pin | CL86T Terminal   | Description |
| ----------- | ---------------- | ----------- |
| Digital 2   | PUL+             | Step pulse  |
| Digital 3   | DIR+             | Direction   |
| Digital 4   | ENA+             | Enable      |
| GND         | PUL-, DIR-, ENA- | Ground      |

| Power Supply | CL86T | Description    |
| ------------ | ----- | -------------- |
| +V (60V)     | VCC   | Power positive |
| -V (GND)     | GND   | Power ground   |

| Motor Wire | CL86T | Description      |
| ---------- | ----- | ---------------- |
| Wire 1     | A+    | Phase A positive |
| Wire 2     | A-    | Phase A negative |
| Wire 3     | B+    | Phase B positive |
| Wire 4     | B-    | Phase B negative |

**⚠️ For detailed wiring instructions, see [Wiring Diagram](docs/wiring-diagram.md)**

## 📁 Project Structure

```
adaptive-drum-kit-cl86t/
├── docs/
│   ├── wiring-diagram.md      # Detailed wiring diagrams
│   ├── hardware-setup.md      # Step-by-step setup guide
│   └── troubleshooting.md    # Common issues and solutions
├── include/
│   └── pins.h                 # Pin definitions and configuration
├── src/
│   └── main.cpp               # Main firmware code
├── platformio.ini             # PlatformIO configuration
└── README.md                  # This file
```

## ⚙️ Configuration

### Pin Configuration

Edit `include/pins.h` to change pin assignments:

```cpp
static const uint8_t PIN_STEP = 2;   // Step pulse pin
static const uint8_t PIN_DIR = 3;    // Direction pin
static const uint8_t PIN_EN = 4;     // Enable pin
```

### Motor Parameters

```cpp
static const long STEPS_PER_REV = 200;  // Steps per revolution
static const float GEAR_RATIO = 1.0f;   // Gear ratio (1.0 = direct drive)
```

### Speed and Acceleration

```cpp
static const float SPEED_MIN_SPS = 100.0f;      // Minimum speed
static const float SPEED_MAX_SPS = 2000.0f;     // Maximum speed
static const float SPEED_DEFAULT_SPS = 500.0f;  // Default speed
static const float ACCEL_SPS2 = 1000.0f;        // Acceleration
```

### Direction and Enable

```cpp
static const bool DIR_INVERT = false;   // Invert direction if needed
static const bool EN_ACTIVE_LOW = false; // Enable active low/high
```

## 📚 Documentation

- **[Wiring Diagram](docs/wiring-diagram.md)** - Complete wiring instructions with diagrams
- **[CL86T Specifications](docs/cl86t-specifications.md)** - Complete CL86T driver specifications
- **[Hardware Setup](docs/hardware-setup.md)** - Step-by-step setup guide
- **[DIP Switch Settings](docs/dip-switch-settings.md)** - DIP switch configuration guide
- **[Microstepping Settings](docs/microstepping-settings.md)** - Complete microstepping options
- **[Current Settings](docs/current-settings.md)** - S1 rotary switch current and gain configuration
- **[Power Supply Selection](docs/power-supply-selection.md)** - Power supply selection and configuration
- **[Optional Connections](docs/optional-connections.md)** - EMI filter, fault output, brake output
- **[Motor Wire Identification](docs/motor-wire-identification.md)** - How to identify and connect motor and encoder wires
- **[Terminal Reference](docs/terminal-reference.md)** - Complete terminal listing for CL86T and LE-350-60
- **[Troubleshooting](docs/troubleshooting.md)** - Common issues and solutions

## 🔧 Development

### Building

```bash
pio run
```

### Uploading

```bash
pio run --target upload
```

### Monitoring Serial

```bash
pio device monitor
```

### Clean Build

```bash
pio run --target clean
```

## 🛠️ Troubleshooting

### Motor Doesn't Move
- Check power supply is on
- Verify all connections
- Check enable signal (try 'E' command)
- Verify motor phase wiring

### Motor Vibrates
- Swap motor phase wires (A+ with A- or B+ with B-)

### Wrong Direction
- Change `DIR_INVERT` to `true` in `pins.h`
- Or swap DIR+ and DIR- connections

### Arduino Not Responding
- Check USB cable
- Verify COM port
- Check serial monitor baud rate (115200)

**For more troubleshooting, see [Troubleshooting Guide](docs/troubleshooting.md)**

## ⚠️ Safety Warnings

1. **High Voltage**: Power supply outputs 60V DC - handle with care
2. **Always disconnect power** before making connections
3. **Double-check wiring** before powering on
4. **Use proper wire gauge** for current capacity
5. **Ensure proper grounding** of all components
6. **Keep work area dry** and free of conductive materials

## 📊 Technical Specifications

| Parameter        | Value           | Description                                 |
| ---------------- | --------------- | ------------------------------------------- |
| **Motor**        | 34E1K-120       | NEMA 34 stepper, 6.0A rated, 12Nm torque    |
| **Encoder**      | Built-in        | 1000 PPR, optical incremental               |
| **Steps/Rev**    | 200             | Full step mode (can be 200-51,200)          |
| **Step Angle**   | 1.8°            | Per step                                    |
| **Driver**       | CL86T           | Closed-loop driver, 24-110VDC, 500KHz max   |
| **Power Supply** | LE-350-60       | 60V DC, 5.8A (within driver range)          |
| **Max Speed**    | 2000 steps/sec  | Configurable (driver supports up to 500KHz) |
| **Acceleration** | 1000 steps/sec² | Configurable                                |
| **Serial Baud**  | 115200          | Communication rate                          |

## 🎯 Next Steps

Once basic motor control is working:

1. **Add Closed-Loop Feedback** - Configure CL86T encoder input
2. **Implement Position Control** - Add position feedback and calibration
3. **Add Sensors** - Integrate force sensors, triggers, etc.
4. **Develop Control Logic** - Implement drum-specific control algorithms
5. **Add Safety Features** - Limit switches, current monitoring, etc.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Arduino community for the robust platform
- PlatformIO for excellent development tools
- AccelStepper library by Mike McCauley
- StepperOnline for hardware components

## 📞 Support

For issues or questions:
- Check [Troubleshooting Guide](docs/troubleshooting.md)
- Review [Hardware Setup Guide](docs/hardware-setup.md)
- Consult component datasheets and manuals

---

**⚠️ Always follow safety guidelines when working with high-voltage power supplies and motors.**

**© 2025 Adaptive Drum Kit Project. All rights reserved.**

