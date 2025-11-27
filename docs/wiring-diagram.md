# Wiring Diagram - CL86T Motor Control System

## Overview

This document provides detailed wiring instructions for connecting the 34E1K-120 stepper motor, CL86T closed-loop driver, LE-350-60 power supply, and Arduino UNO R3.

## Component Specifications

### Hardware Components

| Component           | Model          | Key Specifications                                              |
| ------------------- | -------------- | --------------------------------------------------------------- |
| **Stepper Motor**   | 34E1K-120      | NEMA 34, 1.8° per step, 200 steps/rev, 6.0A rated, 12Nm torque  |
| **Encoder**         | Built-in       | 1000 PPR, optical incremental, differential output              |
| **Driver**          | CL86T          | Closed-loop driver, 24-110VDC, 500KHz max, 200-51,200 steps/rev |
| **Power Supply**    | LE-350-60      | 60V DC, 5.8A output (within CL86T 24-110VDC range)              |
| **Microcontroller** | Arduino UNO R3 | 5V logic, 16MHz                                                 |

**CL86T Driver Features:**
- Input: 18-80VAC or 24-110VDC (our 60V DC is within range)
- Max pulse frequency: 500 KHz
- Resolution: 200-51,200 steps/rev (configurable via DIP switches)
- Optically isolated inputs: 5V or 24V (we use 5V)
- Protection: Over-voltage, over-current, position error detection
- Closed-loop operation: No step loss, automatic error correction

**For complete CL86T specifications, see [CL86T Specifications Guide](cl86t-specifications.md)**

## Wiring Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         POWER SUPPLY                             │
│                         LE-350-60                                │
│                                                                   │
│                    +V ────────────────┐                            │
│                    +V                │                            │
│                    +V                │                            │
│                    -V ───────────────┼───────┐                    │
│                    -V                │       │                    │
│                    -V                │       │                    │
│                    GND               │       │                    │
│                    L, N              │       │                    │
│                                     │       │                    │
└─────────────────────────────────────┼───────┼────────────────────┘
                                       │       │
                                       │       │
┌──────────────────────────────────────┼───────┼────────────────────┐
│                    CL86T DRIVER      │       │                    │
│                                      │       │                    │
│  ┌───────────────────────────────────┘       │                    │
│  │  AC ─────────────────────────────────────┘                    │
│  │  AC ─────────────────────────────────────┐                    │
│  │                                           │                    │
│  │  PUL+ ────────────┐                      │                    │
│  │  PUL- ────────────┼──────────────────────┘                    │
│  │                   │                                            │
│  │  DIR+ ────────────┐                                            │
│  │  DIR- ────────────┼──────────────────────┐                    │
│  │                   │                      │                    │
│  │  ENA+ ────────────┐                      │                    │
│  │  ENA- ────────────┼──────────────────────┼──────────────────┐│
│  │                   │                      │                  ││
│  │  BRK/PEND+         │                      │                  ││
│  │  BRK/PEND-         │                      │                  ││
│  │  ALM+              │                      │                  ││
│  │  ALM-              │                      │                  ││
│  │                    │                      │                  ││
│  │  A+ ───────────────┐                      │                  ││
│  │  A- ───────────────┤                      │                  ││
│  │  B+ ───────────────┤                      │                  ││
│  │  B- ───────────────┤                      │                  ││
│  │                    │                      │                  ││
│  │  EA+ ──────────────┤                      │                  ││
│  │  EA- ──────────────┤                      │                  ││
│  │  EB+ ──────────────┤                      │                  ││
│  │  EB- ──────────────┤                      │                  ││
│  │  VCC ──────────────┤                      │                  ││
│  │  EGND ─────────────┤                      │                  ││
│  └────────────────────┼──────────────────────┼──────────────────┼┘
│                       │                      │                  │
└───────────────────────┼──────────────────────┼──────────────────┼┘
                        │                      │                  │
                        │                      │                  │
┌───────────────────────┼──────────────────────┼──────────────────┼┐
│   ARDUINO UNO R3      │                      │                  ││
│                       │                      │                  ││
│   Digital Pin 2 ──────┘                      │                  ││
│   (PUL+)                                     │                  ││
│                                              │                  ││
│   Digital Pin 3 ────────────────────────────┘                  ││
│   (DIR+)                                                         ││
│                                                                 ││
│   Digital Pin 4 ───────────────────────────────────────────────┘│
│   (ENA+)                                                         │
│                                                                  │
│   GND ──────────────────────────────────────────────────────────┘
│   (to PUL-, DIR-, ENA-)                                          │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
                        │
                        │
┌───────────────────────┼──────────────────────────────────────────┐
│   34E1K-120 MOTOR     │                                          │
│                       │                                          │
│   Black (A+) ─────────┘                                          │
│   Green (A-) ────────┐                                          │
│   Red (B+) ──────────┤                                          │
│   Blue (B-) ─────────┤                                          │
│                       │                                          │
│   Encoder: Red, Black,│                                          │
│   Green, White, Yellow,│                                         │
│   Blue, Black (thick) │                                          │
│                       │                                          │
└───────────────────────┘                                          │
```

## Detailed Connection Guide

### 1. Power Supply Connections

#### LE-350-60 to CL86T Driver

**LE-350-60 Terminals:**
- **+V, +V, +V** (three positive terminals - use any one)
- **-V, -V, -V** (three negative terminals - use any one)
- **GND** (ground/frame ground)
- **N** (neutral - AC input)
- **L** (line/live - AC input)

**AC Input Wire Colors (Typical):**
- **Blue** → **L** (line/live)
- **Brown** → **N** (neutral)
- **Yellow** → **GND** (ground)

**CL86T Power Input Terminals:**
- **AC, AC** (two AC terminals - for AC or DC input, no polarity for DC)

| Power Supply Terminal | CL86T Terminal | Wire Gauge | Notes                  |
| --------------------- | -------------- | ---------- | ---------------------- |
| +V (any one)          | AC (first)     | 14-16 AWG  | Red wire recommended   |
| -V (any one)          | AC (second)    | 14-16 AWG  | Black wire recommended |
| GND                   | (Frame ground) | 14-16 AWG  | Optional, for safety   |

**Note:** CL86T AC terminals accept DC input (24-110VDC). No polarity requirement for DC input.

**Important Safety Notes:**
- Ensure power supply is OFF before making connections
- Double-check polarity - reversed polarity can damage the driver
- Use appropriate wire gauge for current capacity (5.8A max)
- Ensure all connections are secure and properly insulated

### 2. Motor Connections

#### 34E1K-120 Motor Specifications

| Parameter               | Value                | Notes         |
| ----------------------- | -------------------- | ------------- |
| **Rated Current/Phase** | 6.0A                 | Per phase     |
| **Phase Resistance**    | 0.72Ω                | Per phase     |
| **Inductance**          | 7.3mH ± 20%          | At 1KHz       |
| **Holding Torque**      | 12Nm (1699.34 oz.in) |               |
| **Step Angle**          | 1.8°                 | 200 steps/rev |
| **Frame Size**          | NEMA 34 (86 x 86mm)  |               |
| **Body Length**         | 187.6mm              |               |
| **Shaft Diameter**      | 14mm                 |               |
| **Weight**              | 5.0kg                |               |

#### Motor Power Wires to CL86T Driver

The 34E1K-120 motor has 4 power wires. Connect them to the CL86T driver as follows:

**✅ Verified Configuration:**

| Motor Wire Color | CL86T Terminal | Pin Number | Function         | Notes      |
| ---------------- | -------------- | ---------- | ---------------- | ---------- |
| **Black**        | A+             | Pin 1      | Phase A positive | ✅ Verified |
| **Green**        | A-             | Pin 2      | Phase A negative | ✅ Verified |
| **Red**          | B+             | Pin 3      | Phase B positive | ✅ Verified |
| **Blue**         | B-             | Pin 4      | Phase B negative | ✅ Verified |

**Connection Steps:**
1. Connect **Black** wire to CL86T **A+** terminal
2. Connect **Green** wire to CL86T **A-** terminal
3. Connect **Red** wire to CL86T **B+** terminal
4. Connect **Blue** wire to CL86T **B-** terminal

**Motor Wire Identification:**
- ✅ **This is the verified configuration for 34E1K-120 motor**
- If motor vibrates but doesn't rotate, swap A+ and A- (Black ↔ Green) or B+ and B- (Red ↔ Blue)
- If your motor has different colors, use multimeter to identify pairs (~0.72Ω per phase)
- **For detailed wire identification, see [Motor Wire Identification Guide](motor-wire-identification.md)**

#### Encoder Connections (34E1K-120 has built-in encoder)

The 34E1K-120 includes a 1000 PPR (pulses per revolution) optical incremental encoder with differential outputs. Connect to CL86T encoder terminals:

| Encoder Wire     | CL86T Terminal | Description                    | Notes                      |
| ---------------- | -------------- | ------------------------------ | -------------------------- |
| VCC (5V)         | VCC            | Encoder +5V voltage **OUTPUT** | ⚠️ **Driver supplies 5V**   |
| EGND (Ground)    | EGND           | Power ground connection        | Encoder ground             |
| EA+ (Encoder A+) | EA+            | Encoder A+ input connection    | Encoder channel A positive |
| EA- (Encoder A-) | EA-            | Encoder A- input connection    | Encoder channel A negative |
| EB+ (Encoder B+) | EB+            | Encoder B+ input connection    | Encoder channel B positive |
| EB- (Encoder B-) | EB-            | Encoder B- input connection    | Encoder channel B negative |

**Important:** The CL86T **VCC terminal provides 5V OUTPUT** to power the encoder. Do not connect external 5V power to this terminal.

**Encoder Specifications:**
- **Type:** Optical Incremental
- **Resolution:** 1000 PPR
- **Output Type:** Differential
- **Supply Voltage:** 5V (supplied by CL86T VCC terminal)
- **Output High:** <4V
- **Output Low:** <1V
- **Channels:** 2 (A and B quadrature)

**Encoder Wire Colors (Typical):**
- **Red:** VCC (+5V power - driver supplies)
- **Black (thin):** EGND (ground)
- **Green:** EA+ (encoder A positive)
- **White:** EA- (encoder A negative)
- **Yellow:** EB+ (encoder B positive)
- **Blue:** EB- (encoder B negative)
- **Black (thick):** Shield/ground (optional)

**Encoder Connection Notes:**
- The encoder uses differential signaling for noise immunity
- Connect both + and - signals for each channel
- **CL86T VCC terminal supplies 5V to encoder** - do not connect external 5V
- Keep encoder wires away from motor power wires to reduce interference
- Use shielded cable for encoder connections if possible
- The encoder enables closed-loop operation on the CL86T driver
- **For detailed wire identification, see [Motor Wire Identification Guide](motor-wire-identification.md)**

### 3. Arduino to CL86T Driver Connections

#### Control Signal Connections

**CL86T Control Signal Terminals:**
- **PUL+, PUL-** (Pulse/Step signal)
- **DIR+, DIR-** (Direction signal)
- **ENA+, ENA-** (Enable signal, optional)

| Arduino Pin   | CL86T Terminal | Wire Type | Notes                    |
| ------------- | -------------- | --------- | ------------------------ |
| Digital Pin 2 | PUL+           | 22-24 AWG | Step/Pulse signal        |
| Digital Pin 3 | DIR+           | 22-24 AWG | Direction signal         |
| Digital Pin 4 | ENA+           | 22-24 AWG | Enable signal (optional) |
| GND           | PUL-           | 22-24 AWG | Common ground            |
| GND           | DIR-           | 22-24 AWG | Common ground            |
| GND           | ENA-           | 22-24 AWG | Common ground            |

**Signal Connection Details:**
- The CL86T uses opto-isolated inputs
- **Connection Types Supported:**
  - Differential or single-ended control signals
  - Open-collector or PNP connection
  - Our setup: Single-ended (Arduino digital pins to + terminals, GND to - terminals)
- The "+" terminals connect to Arduino digital pins
- The "-" terminals connect to Arduino GND
- **Signal Voltage:** High level 3.5-5V, low level 0-0.5V (we use 5V)
- **5V/24V Switch:** ⚠️ **Must set rotating switch to 5V** (factory default is 24V)
- **Pulse Signal Requirements:**
  - Pulse width: Minimum 1.0μs
  - Duty cycle: Recommended 50%
  - Maximum frequency: 500 KHz (5V mode)
- **Direction Signal:** Must advance PUL signal by minimum 2μs in single pulse mode
- **Enable Signal:**
  - Optional (no connection as default)
  - Available for 5V-24V
  - Must advance DIR signal by minimum 200ms (if used)
- **EMI Filtering:** ⚠️ **Recommended:** Add EMI line filter between power supply and driver for noise immunity in interference environments
- Optional: Add 1kΩ resistors in series for additional protection (not always required)
- Use twisted pair or shielded cable for longer runs (>1 meter)

### 4. Optional Resistor Protection

For additional protection, you can add 1kΩ resistors in series:

```
Arduino Pin 2 ──[1kΩ]── PUL+
Arduino Pin 3 ──[1kΩ]── DIR+
Arduino Pin 4 ──[1kΩ]── ENA+
```

**Note:** The CL86T's opto-isolators typically have built-in current limiting, so resistors are optional but recommended for best practice.

## Pin Assignment Summary

### Arduino UNO R3 Pinout

```
                    ┌───────┐
              GND ──┤ 1   2 │── 5V
         Digital 0 ──┤ 3   4 │── Digital 1
         Digital 2 ──┤ 5   6 │── Digital 3  ← DIR
         Digital 4 ──┤ 7   8 │── Digital 5  ← EN
         Digital 6 ──┤ 9  10 │── Digital 7
         Digital 8 ──┤11  12 │── Digital 9
        Digital 10 ──┤13  14 │── Digital 11
        Digital 12 ──┤15  16 │── Digital 13
              AREF ──┤17  18 │── A0
               A1 ──┤19  20 │── A2
               A3 ──┤21  22 │── A4
               A5 ──┤23  24 │──
                    └───────┘
```

**Used Pins:**
- **Pin 2**: STEP (PUL+) - Step pulse output
- **Pin 3**: DIR (DIR+) - Direction control
- **Pin 4**: EN (ENA+) - Enable control
- **GND**: Common ground for all signals

## CL86T Driver Terminal Layout

**Complete Terminal List:**

**Power Input:**
- **AC, AC** - Power input (AC 18-80VAC or DC 24-110VDC, no polarity for DC)

**Control Signals (Input):**
- **PUL+, PUL-** - Pulse/Step signal (optically isolated)
- **DIR+, DIR-** - Direction signal (optically isolated)
- **ENA+, ENA-** - Enable signal (optically isolated, optional)

**Output Signals:**
- **BRK/PEND+, BRK/PEND-** - Brake/Pend output (max 30VDC/100mA, requires relay for brake)
- **ALM+, ALM-** - Alarm/Fault output (max 30VDC/100mA, optional monitoring)

**Motor Output:**
- **A+, A-** - Motor phase A
- **B+, B-** - Motor phase B

**Encoder Connections (Input):**
- **EA+, EA-** - Encoder channel A (differential input)
- **EB+, EB-** - Encoder channel B (differential input)
- **VCC, EGND** - Encoder power (VCC supplies 5V OUTPUT, EGND is ground)

**Visual Layout:**
```
┌─────────────────────────────────────┐
│          CL86T DRIVER               │
│                                     │
│  [AC] [AC]                          │  ← Power input
│                                     │
│  [PUL+] [PUL-]                      │  ← Step signal
│  [DIR+] [DIR-]                      │  ← Direction signal
│  [ENA+] [ENA-]                      │  ← Enable signal
│                                     │
│  [BRK/PEND+] [BRK/PEND-]            │  ← Brake/Pend output
│  [ALM+] [ALM-]                      │  ← Alarm output
│                                     │
│  [A+] [A-] [B+] [B-]                │  ← Motor output
│                                     │
│  [EA+] [EA-]                        │  ← Encoder A
│  [EB+] [EB-]                        │  ← Encoder B
│  [VCC] [EGND]                       │  ← Encoder power (5V OUTPUT)
│                                     │
│  [LED: GREEN] [LED: RED]            │  ← Status indicators
│                                     │
└─────────────────────────────────────┘
```

**Terminal Functions:**
- **AC, AC:** Power input (AC 18-80VAC or DC 24-110VDC, no polarity for DC)
- **PUL+/PUL-:** Pulse/Step signal (Input, optically isolated)
- **DIR+/DIR-:** Direction signal (Input, optically isolated)
- **ENA+/ENA-:** Enable signal (Input, optional, optically isolated)
- **BRK/PEND+/BRK/PEND-:** Brake/Pend output (Output, max 30VDC/100mA, requires relay for brake)
- **ALM+/ALM-:** Alarm/Fault output (Output, max 30VDC/100mA, optional monitoring)
- **A+/A-/B+/B-:** Motor phase connections
- **EA+/EA-/EB+/EB-:** Encoder differential inputs
- **VCC/EGND:** Encoder 5V power output (driver supplies power to encoder)

## LED Status Indicators

The CL86T has two LED status lights:

| LED Color | Status             | Meaning                                               |
| --------- | ------------------ | ----------------------------------------------------- |
| **GREEN** | Always ON (normal) | Power indicator - should be on when driver is powered |
| **RED**   | OFF (normal)       | Alarm indicator - OFF during normal operation         |
| **RED**   | ON and flashing    | Alarm active - indicates protection triggered         |

**Alarm Conditions (RED LED flashing):**
- Over-voltage protection activated
- Over-current protection activated
- Position following error detected
- Other protection mechanisms triggered

**If RED LED is flashing:**
1. Check power supply voltage (should be 24-110VDC)
2. Check current setting (may be too high)
3. Check motor connections
4. Check encoder connections (if using closed-loop)
5. Refer to CL86T manual for specific alarm codes

## Fault Output Connection (Optional)

The CL86T provides fault/alarm output signals that can be monitored:

### ALM+/ALM- Terminal

**Function:**
- **Output:** Fault/alarm status indication
- **Voltage:** Max 30VDC/100mA
- **Connection:** Optional - can be connected for monitoring

**How It Works:**
- When fault occurs (over-voltage, over-current, etc.):
  - RED LED blinks
  - Impedance state between ALM+ and ALM- changes
  - Can be detected by external circuit (Arduino, PLC, etc.)

**Connection Options:**
- **Sinking:** Connect ALM- to load, ALM+ to supply
- **Sourcing:** Connect ALM+ to load, ALM- to ground
- Can be connected to Arduino digital input (with appropriate voltage level conversion if needed)

**Use Cases:**
- Monitor driver status remotely
- Trigger safety shutdowns
- Log fault conditions
- Integrate with control system

**Note:** This is optional - driver will still function without ALM connection. RED LED provides visual indication.

## Brake Output Connection (Optional)

The CL86T provides brake output for motor brake control:

### BRK+/BRK- Terminal

**Function:**
- **Output:** Brake control signal
- **Voltage:** Max 30VDC/100mA
- **Connection:** Requires relay to drive motor brake
- **Mode:** Selected by SW8 (OFF = Brake output, ON = Pend output)

**Connection Diagram:**
```
CL86T Driver          Relay              Motor Brake
┌──────────┐         ┌──────┐           ┌──────────┐
│ BRK+     │─────────│ Coil │           │          │
│ BRK-     │─────────│      │           │  Brake   │
└──────────┘         └──────┘           │  Coil    │
                         │               │          │
                         └───────────────┘          │
                                                    │
Power Supply ──────────────────────────────────────┘
```

**Important Notes:**
- **Requires relay:** Brake output cannot directly drive motor brake
- **Relay selection:** Choose relay appropriate for brake coil voltage/current
- **Isolation:** Relay provides electrical isolation between driver and brake
- **SW8 setting:** Set SW8 to OFF for brake output mode

**Use Cases:**
- Motor holding brake (prevents movement when power off)
- Safety brake activation
- Controlled brake release

**Note:** This is optional - only needed if motor has brake and you want automatic control.

## EMI Filter Recommendation

**⚠️ Recommended for Noise Immunity:**

Add an EMI (Electromagnetic Interference) line filter between the power supply and CL86T driver:

```
Power Supply ──[EMI Filter]── CL86T Driver
```

**Why:**
- Reduces electrical noise
- Improves drive reliability in interference environments
- Prevents false triggering of protection circuits
- Better signal integrity

**When to Use:**
- Industrial environments with electrical noise
- Multiple motors/drivers in same system
- Long power supply cables
- Sensitive applications

**Filter Selection:**
- Rated for power supply voltage (60V DC in our case)
- Rated for current capacity (5.8A minimum)
- DC-rated filter for DC power supply
- Install as close to driver as possible

**Note:** Not strictly required, but recommended for best performance in noisy environments.

## Power Supply Setup

### LE-350-60 Power Supply

**Physical Specifications:**
- **Model:** LE-350-60 S Single Output
- **Dimensions:** 215 × 115 × 30mm (8.5" × 4.5" × 1.2")
- **Installation:** Vertical or horizontal mounting
- **Cooling:** Fan cooling

**Input Specifications:**
- **Input Voltage:** 115/230V AC ±20% (auto-select or manual)
- **Input Frequency:** 47-63 Hz
- **Inrush Current:** 40A @ 230VAC (cold state)
- **EMI Filtering:** Internal EMI filter included

**Output Specifications:**
- **Output Voltage:** 60V DC (adjustable ±10%: 54V-66V)
- **Output Current:** 5.8A maximum
- **Rated Power:** 350W
- **Voltage Stability:** 0.5%
- **Ripple and Noise:** ≤1% Vout (≤0.6V)
- **Efficiency:** ≥89.5%
- **Rise Time:** 50ms @ 230VAC, full load
- **Holding Time:** 20ms @ 230VAC, full load

**Protection Features:**
- **Short Circuit Protection:** Self-recovery
- **Overload Protection:** 5 seconds @ 180-220% (self-recovery)
- **Mean Time Between Failures:** MTBF ≥ 500,000 hours

**Safety Specifications:**
- **Dielectric Strength:** 1500V AC (input-output, input-ground), 500V AC (output-ground)
- **Insulation Resistance:** ≥100MΩ
- **Leakage Current:** <0.5mA @ 230VAC
- **Safety Standards:** GB4943, EN60950, UL60950 compliant
- **EMC Standards:** EN55022 Class C, EN61000 series

**Environmental Specifications:**
- **Operating Temperature:**
  - 0-45°C @ 100% load
  - -10°C @ 80% load
  - 60°C @ 40% load
- **Storage Temperature:** -20°C to 85°C
- **Relative Humidity:** 20-90% RH (no condensation)

**Connection Steps:**
1. **Mount power supply:** Vertical or horizontal, ensure fan clearance
2. **Connect AC input:** To wall outlet (115/230V AC, verify voltage)
3. **Set output voltage:** 60V DC (adjustable ±10% if needed)
4. **Connect DC output:** To CL86T VCC and GND terminals
5. **Verify polarity:** Check with multimeter before powering on
6. **Ensure ventilation:** Fan cooling requires adequate airflow

**Safety:**
- Use appropriate AC fuse/breaker
- Ensure proper grounding (frame ground)
- Keep power supply in well-ventilated area (fan cooling)
- Do not exceed maximum current rating (5.8A)
- Monitor current draw (close to limit with 5.7A motor)
- Allow space for heat dissipation
- **Warranty:** 1 year manufacturer warranty

## Complete Wiring Checklist

Before powering on, verify:

- [ ] Power supply AC input connected and voltage set correctly
- [ ] Power supply DC output connected to CL86T (VCC to +, GND to -)
- [ ] Motor wires connected to CL86T (A+, A-, B+, B-)
- [ ] Arduino Pin 2 connected to CL86T PUL+
- [ ] Arduino Pin 3 connected to CL86T DIR+
- [ ] Arduino Pin 4 connected to CL86T ENA+ (optional)
- [ ] Arduino GND connected to CL86T PUL-, DIR-, ENA-
- [ ] All connections are secure and properly insulated
- [ ] No short circuits between terminals
- [ ] Motor shaft is free to rotate
- [ ] Arduino USB connected to computer (for programming/monitoring)

## Troubleshooting Connections

### Motor Doesn't Move
- Check power supply is on and outputting correct voltage
- Verify motor wires are connected correctly (try swapping A+ and A-)
- Check Arduino connections to CL86T
- Verify enable signal is active (check PIN_EN in code)

### Motor Vibrates but Doesn't Rotate
- Swap motor phase wires (A+ with A- or B+ with B-)
- Check motor wire continuity with multimeter
- Verify driver current settings match motor specifications

### Motor Moves in Wrong Direction
- Change `DIR_INVERT` in `pins.h` to `true`
- Or swap DIR+ and DIR- connections

### Arduino Not Communicating
- Check USB cable connection
- Verify correct COM port selected
- Check serial monitor baud rate (115200)
- Ensure Arduino is powered (LED should be on)

## Additional Resources

- CL86T Manual: See `CL86T (V4.0) manual.pdf` in project directory
- 34E1K-120 Datasheet: See `34E1K-120_Full_Datasheet.pdf` in project directory
- LE-350-60 Datasheet: See `LE-350-60.pdf` in project directory

