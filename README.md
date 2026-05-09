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
- **Flexible Commands**: Nudge, home, map an angle, trigger, and return
- **Speed Control**: Adjustable speed with real-time changes
- **Position Control**: Move to specific positions or home
- **Enable Control**: Firmware supports optional enable output, but `ENA+` is not wired during bring-up
- **FSR Input**: Current firmware reads a DF9-16 FSR on A0
- **Load Sensors**: Support for up to 2 strain gauge load cells (HX711)
- **Sensor Calibration**: Built-in calibration commands for all sensors
- **Well Documented**: Consolidated hardware setup and wiring guide

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

2. **Build once to install PlatformIO dependencies**
   ```bash
   pio run
   ```

   Dependencies are declared in `platformio.ini` under `lib_deps`, so PlatformIO installs them automatically.

3. **Connect hardware** (see [Hardware Setup and Wiring Guide](docs/hardware-setup.md))
   - Connect power supply to CL86T driver
   - Connect motor to CL86T driver
   - Connect Arduino to CL86T driver (`D2 -> PUL+`, `D3 -> DIR+`, `GND -> PUL-`, `DIR-`, and `ENA-`)
   - Leave Arduino Pin 4 disconnected from `ENA+` for initial bring-up

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
| `P` or `p` | Read FSR sensor value on A0                                   | `P`           |
| `L` or `l` | Read load sensor values                                        | `L`           |
| `C` or `c` | Calibrate all sensors (set zero/baseline)                      | `C`           |
| `D` or `d` | Diagnostic mode - continuous A0 reading                        | `D`           |
| `A` or `a` | Toggle FSR auto-trigger mode                                   | `A`           |
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
| Digital 4   | Not connected during bring-up | Reserved for optional future ENA+ control |
| GND         | PUL-, DIR-, ENA- | Ground      |

| Power Supply | CL86T | Description    |
| ------------ | ----- | -------------- |
| +V (60V)     | AC    | DC input to non-polarized CL86T AC terminal |
| -V (GND)     | AC    | DC input to non-polarized CL86T AC terminal |

| Motor Wire | CL86T | Description      |
| ---------- | ----- | ---------------- |
| Black      | A+    | Phase A positive |
| Green      | A-    | Phase A negative |
| Red        | B+    | Phase B positive |
| Blue       | B-    | Phase B negative |

| Arduino Pin | Sensor Component | Description |
| ----------- | ---------------- | ----------- |
| Analog A0   | DF9-16 FSR | Voltage divider with 10k resistor |
| Digital 5   | HX711 #1 DT      | Load Sensor 1 Data |
| Digital 6   | HX711 #1 SCK     | Load Sensor 1 Clock |
| Digital 7   | HX711 #2 DT      | Load Sensor 2 Data |
| Digital 8   | HX711 #2 SCK     | Load Sensor 2 Clock |
| 5V          | HX711 VCC        | Power for HX711 modules |
| GND         | All sensor grounds | Common ground |

**⚠️ For all wiring, driver settings, sensors, and troubleshooting, see [Hardware Setup and Wiring Guide](docs/hardware-setup.md).**

## 📁 Project Structure

```
adaptive-drum-kit-cl86t/
├── docs/
│   ├── hardware-setup.md      # Consolidated hardware, wiring, setup, and troubleshooting guide
│   └── code-proposal-ip-alignment.md # Future architecture proposal
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
static const uint8_t PIN_EN = 4;     // Reserved for optional future ENA+ control
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

- **[Hardware Setup and Wiring Guide](docs/hardware-setup.md)** - Consolidated wiring, CL86T setup, sensors, first power-on, and troubleshooting
- **[Code Proposal: IP Alignment](docs/code-proposal-ip-alignment.md)** - Future architecture proposal

## 🔧 Development

### PlatformIO Environments

The default environment is `uno_official`.

```bash
pio run
pio run --environment uno_sanesmart
```

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
- Disconnect Arduino Pin 4 from `ENA+` if present; keep `ENA-` grounded
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

**For more troubleshooting, see [Hardware Setup and Wiring Guide](docs/hardware-setup.md).**

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

1. **Connect Sensors** - Wire up the DF9-16 FSR and load sensors (see [Hardware Setup and Wiring Guide](docs/hardware-setup.md))
2. **Calibrate Sensors** - Use 'C' command to calibrate all sensors
3. **Test Sensors** - Use 'P' and 'L' commands to read sensor values
4. **Expand Feedback** - Add firmware-level use of driver alarm/status outputs if needed
5. **Implement Position Calibration** - Add travel limits and beater contact calibration
6. **Develop Control Logic** - Integrate sensor readings with motor control
7. **Add Safety Features** - Limit switches, current monitoring, etc.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Arduino community for the robust platform
- PlatformIO for excellent development tools
- AccelStepper library by Mike McCauley
- StepperOnline for hardware components

## 📞 Support

For issues or questions:
- Review [Hardware Setup and Wiring Guide](docs/hardware-setup.md)
- Consult component datasheets and manuals

---

**⚠️ Always follow safety guidelines when working with high-voltage power supplies and motors.**

**© 2026 Adaptive Drum Kit Project. All rights reserved.**

