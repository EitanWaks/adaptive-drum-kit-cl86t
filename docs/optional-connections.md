# Optional Connections Guide

## Overview

The CL86T driver supports several optional connections for enhanced functionality: fault monitoring, brake control, and EMI filtering. This guide covers these optional features.

## EMI Line Filter (Recommended)

### Purpose

An EMI (Electromagnetic Interference) line filter reduces electrical noise between the power supply and driver, improving reliability in interference environments.

### Installation

**Connection:**
```
Power Supply ──[EMI Filter]── CL86T Driver (VCC/GND)
```

**Filter Selection:**
- **Voltage Rating:** Must handle power supply voltage (60V DC in our case)
- **Current Rating:** Must handle maximum current (5.8A minimum)
- **Type:** DC-rated filter for DC power supply
- **Installation:** As close to driver as possible

### Benefits

- ✅ Reduces electrical noise
- ✅ Improves drive reliability
- ✅ Prevents false triggering of protection circuits
- ✅ Better signal integrity
- ✅ Especially important in industrial/noisy environments

### When to Use

**Recommended for:**
- Industrial environments with electrical noise
- Multiple motors/drivers in same system
- Long power supply cables
- Sensitive applications
- When experiencing unexplained faults or erratic behavior

**Not Required For:**
- Simple single-motor setups
- Clean electrical environments
- Short power supply cables
- Basic applications

### Installation Steps

1. **Select appropriate EMI filter:**
   - 60V DC, 5.8A minimum rating
   - DC-rated filter

2. **Install between power supply and driver:**
   - Power supply output → Filter input
   - Filter output → CL86T VCC/GND

3. **Mount filter securely:**
   - Close to driver if possible
   - Ensure proper grounding

## Fault Output (ALM+/ALM-)

### Purpose

The fault output provides electrical indication of driver alarm conditions, allowing remote monitoring and integration with control systems.

### Specifications

- **Output Type:** Open-collector or PNP
- **Voltage:** Max 30VDC
- **Current:** Max 100mA
- **Connection:** Optional (driver works without it)

### How It Works

**Normal Operation:**
- RED LED: OFF
- ALM+/ALM-: One impedance state

**Fault Condition:**
- RED LED: ON and flashing
- ALM+/ALM-: Impedance state changes (low to high or high to low)
- Can be detected by external circuit

### Connection Methods

#### Sinking Connection

```
ALM+ ──[Load]── ALM-
                │
              Ground
```

#### Sourcing Connection

```
Power ──[Load]── ALM+
                  │
                 ALM-
                 │
               Ground
```

### Arduino Connection Example

**With Voltage Level Conversion (if needed):**

```
CL86T ALM+ ──[Resistor]── Arduino Digital Pin
CL86T ALM- ─────────────── Arduino GND
```

**Note:** Check voltage levels - ALM output may need level conversion for Arduino 5V logic.

### Use Cases

- **Remote Monitoring:** Detect faults from control system
- **Safety Shutdowns:** Trigger safety circuits on fault
- **Fault Logging:** Record fault conditions
- **System Integration:** Integrate with PLC or other controllers
- **Diagnostics:** Automated fault detection

### Fault Conditions

The ALM output activates for:
- Over-voltage protection
- Over-current protection
- Position following error
- Other protection mechanisms

**Note:** RED LED provides visual indication even without ALM connection.

## Brake Output (BRK+/BRK-)

### Purpose

The brake output provides control signal for motor brake, allowing automatic brake control when motor is enabled/disabled.

### Specifications

- **Output Type:** Open-collector or PNP
- **Voltage:** Max 30VDC
- **Current:** Max 100mA
- **Requires:** Relay (cannot directly drive brake)
- **Mode:** Selected by SW8 (OFF = Brake, ON = Pend)

### Connection Diagram

```
CL86T Driver          Relay              Motor Brake
┌──────────┐         ┌──────┐           ┌──────────┐
│ BRK+     │─────────│ Coil │           │          │
│          │         │      │           │  Brake   │
│ BRK-     │─────────│      │           │  Coil    │
└──────────┘         └──────┘           │          │
                         │               │          │
                         └───────────────┘          │
                                                    │
Power Supply ──────────────────────────────────────┘
```

### Relay Selection

**Requirements:**
- Coil voltage: 5-24V DC (match BRK output)
- Coil current: <100mA
- Contact rating: Match brake coil voltage/current
- Type: DC relay for DC brake coil

**Example:**
- If brake coil is 24V DC, 1A:
  - Use 24V DC relay
  - Relay contacts rated for 24V DC, 1A minimum

### SW8 Setting

- **SW8 = OFF:** Brake output mode (our setting)
- **SW8 = ON:** Pend output mode (different function)

### Use Cases

- **Motor Holding Brake:** Prevents movement when power off
- **Safety Brake:** Automatic brake activation on fault
- **Controlled Release:** Brake releases when motor enabled
- **Position Locking:** Brake holds position when motor disabled

### When to Use

**Use brake output if:**
- Motor has integrated brake
- Automatic brake control needed
- Safety requires brake activation
- Position must be held when disabled

**Not needed if:**
- Motor has no brake
- Manual brake control is sufficient
- Brake not required for application

### Installation Steps

1. **Set SW8 to OFF** (Brake output mode)

2. **Select appropriate relay:**
   - Match brake coil voltage/current
   - Coil voltage: 5-24V DC
   - Coil current: <100mA

3. **Connect relay:**
   - CL86T BRK+ → Relay coil +
   - CL86T BRK- → Relay coil -
   - Relay contacts → Motor brake coil
   - Power supply → Motor brake coil

4. **Test operation:**
   - Verify brake releases when motor enabled
   - Verify brake engages when motor disabled

## Connection Summary

| Connection | Required | Purpose | Complexity |
|------------|----------|---------|------------|
| **EMI Filter** | Recommended | Noise reduction | Low |
| **Fault Output (ALM)** | Optional | Fault monitoring | Medium |
| **Brake Output (BRK)** | Optional | Brake control | Medium (needs relay) |

## Quick Reference

### EMI Filter
- **Install:** Between power supply and driver
- **Rating:** 60V DC, 5.8A minimum
- **Benefit:** Noise reduction, better reliability

### Fault Output (ALM)
- **Connect:** To monitoring circuit (Arduino, PLC, etc.)
- **Voltage:** Max 30VDC/100mA
- **Benefit:** Remote fault detection

### Brake Output (BRK)
- **Connect:** Through relay to motor brake
- **Voltage:** Max 30VDC/100mA
- **Requires:** Relay (cannot drive brake directly)
- **Benefit:** Automatic brake control

## Additional Resources

- **Wiring Diagram:** See [Wiring Diagram](wiring-diagram.md) for complete connection details
- **Hardware Setup:** See [Hardware Setup Guide](hardware-setup.md) for installation steps
- **CL86T Manual:** See `CL86T (V4.0) manual.pdf` for complete specifications

