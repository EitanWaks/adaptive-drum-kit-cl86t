# Terminal Reference Guide

## Complete Terminal Listings

This document provides a complete reference of all terminals on the CL86T driver and LE-350-60 power supply.

## CL86T Driver Terminals

### Power Input
| Terminal | Function | Type | Notes |
|----------|----------|------|-------|
| **AC** | Power input (first) | Input | AC 18-80VAC or DC 24-110VDC |
| **AC** | Power input (second) | Input | No polarity requirement for DC |

### Control Signals (Input)
| Terminal | Function | Type | Notes |
|----------|----------|------|-------|
| **PUL+** | Pulse/Step signal positive | Input | Optically isolated, 5V or 24V |
| **PUL-** | Pulse/Step signal negative | Input | Connect to GND |
| **DIR+** | Direction signal positive | Input | Optically isolated, 5V or 24V |
| **DIR-** | Direction signal negative | Input | Connect to GND |
| **ENA+** | Enable signal positive | Input | Optically isolated, optional, 5V or 24V |
| **ENA-** | Enable signal negative | Input | Connect to GND |

### Output Signals
| Terminal | Function | Type | Notes |
|----------|----------|------|-------|
| **BRK/PEND+** | Brake/Pend output positive | Output | Max 30VDC/100mA, requires relay |
| **BRK/PEND-** | Brake/Pend output negative | Output | Max 30VDC/100mA |
| **ALM+** | Alarm/Fault output positive | Output | Max 30VDC/100mA, optional |
| **ALM-** | Alarm/Fault output negative | Output | Max 30VDC/100mA |

### Motor Output
| Terminal | Function | Type | Notes |
|----------|----------|------|-------|
| **A+** | Motor phase A positive | Output | Connect to motor A+ wire |
| **A-** | Motor phase A negative | Output | Connect to motor A- wire |
| **B+** | Motor phase B positive | Output | Connect to motor B+ wire |
| **B-** | Motor phase B negative | Output | Connect to motor B- wire |

### Encoder Connections
| Terminal | Function | Type | Notes |
|----------|----------|------|-------|
| **EA+** | Encoder channel A positive | Input | Differential input |
| **EA-** | Encoder channel A negative | Input | Differential input |
| **EB+** | Encoder channel B positive | Input | Differential input |
| **EB-** | Encoder channel B negative | Input | Differential input |
| **VCC** | Encoder power supply | Output | ⚠️ **Driver supplies 5V** - do not connect external 5V |
| **EGND** | Encoder ground | Ground | Encoder ground connection |

## LE-350-60 Power Supply Terminals

### AC Input
| Terminal | Function | Type | Notes |
|----------|----------|------|-------|
| **L** | Line/Live | Input | Hot wire from wall outlet |
| **N** | Neutral | Input | Neutral wire from wall outlet |
| **GND** | Ground | Input | Safety ground wire |

**AC Input Wire Colors:**
- **Blue** → **L** (line/live)
- **Brown** → **N** (neutral)
- **Yellow** → **GND** (ground)

### DC Output
| Terminal | Function | Type | Notes |
|----------|----------|------|-------|
| **+V** | Positive output (first) | Output | 60V DC, 5.8A max |
| **+V** | Positive output (second) | Output | Use any one of three |
| **+V** | Positive output (third) | Output | All three are connected |
| **-V** | Negative output (first) | Output | Ground, 5.8A max |
| **-V** | Negative output (second) | Output | Use any one of three |
| **-V** | Negative output (third) | Output | All three are connected |

**Note:** Multiple +V and -V terminals are provided for convenience - use any one of each.

## Connection Summary

### Power Supply to CL86T
```
LE-350-60 +V (any one)  →  CL86T AC (first terminal)
LE-350-60 -V (any one)  →  CL86T AC (second terminal)
LE-350-60 GND           →  (Frame ground, optional)
```

### Arduino to CL86T
```
Arduino Pin 2  →  CL86T PUL+
Arduino Pin 3  →  CL86T DIR+
Arduino Pin 4  →  CL86T ENA+  (optional)
Arduino GND    →  CL86T PUL-, DIR-, ENA-
```

### Motor to CL86T (✅ Verified)
```
Motor Black   →  CL86T A+
Motor Green   →  CL86T A-
Motor Red     →  CL86T B+
Motor Blue    →  CL86T B-
```

### Encoder to CL86T
```
Encoder Red         →  CL86T VCC   (driver supplies 5V)
Encoder Black (thin) →  CL86T EGND
Encoder Green       →  CL86T EA+
Encoder White       →  CL86T EA-
Encoder Yellow      →  CL86T EB+
Encoder Blue        →  CL86T EB-
Encoder Black (thick) →  CL86T EGND (shield, optional)
```

## Terminal Layout Quick Reference

### CL86T Driver
```
Power:        AC, AC
Control:      PUL+, PUL-, DIR+, DIR-, ENA+, ENA-
Outputs:      BRK/PEND+, BRK/PEND-, ALM+, ALM-
Motor:        A+, A-, B+, B-
Encoder:      EA+, EA-, EB+, EB-, VCC, EGND
```

### LE-350-60 Power Supply
```
AC Input:     L, N, GND
  Wire Colors: Blue → L, Brown → N, Yellow → GND
DC Output:    +V, +V, +V, -V, -V, -V
```

## Additional Resources

- **Wiring Diagram:** See [Wiring Diagram](wiring-diagram.md) for complete connection diagrams
- **Hardware Setup:** See [Hardware Setup Guide](hardware-setup.md) for step-by-step instructions
- **Motor Wire Identification:** See [Motor Wire Identification Guide](motor-wire-identification.md) for wire color codes

