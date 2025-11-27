# CL86T Driver Specifications

## Overview

The CL86T is a closed-loop stepper motor driver with advanced features for precision control and reliability.

## Key Features

### Power Supply

- **Input Voltage Range:**
  - AC: 18-80VAC
  - DC: 24-110VDC
- **Our Configuration:**
  - Using LE-350-60 power supply: 60V DC
  - Within driver's 24-110VDC range ✅
  - Provides good headroom for operation

### Performance Specifications

- **Maximum Pulse Frequency:**
  - **5V input mode:** 500 KHz (500,000 pulses/second) ⭐ We use this mode
  - **24V input mode:** 200 KHz (200,000 pulses/second)
  - Our code uses much lower speeds (100-2000 steps/sec)
  - Plenty of headroom for future high-speed applications
- **Resolution Range:** 200-51,200 steps/revolution
  - Configured via DIP switches SW1-SW4
  - Our setting: 200 steps/rev (full step mode)
  - Can be increased for higher resolution if needed

### Current Control

- **Current Settings:** 2 output current settings via S1 rotary switch
- **Gain Tuning:** Adjustable via S1 rotary switch
- **Current Ranges:**
  - Positions 0-3: 4.0A RMS (5.6A peak)
  - Positions 4-9: 5.0A RMS (7.0A peak)
  - Positions A-F: 5.7A RMS (8.0A peak)

### Input Configuration

- **Optically Isolated Inputs:**
  - Supports 5V or 24V logic levels
  - Our configuration: 5V (Arduino UNO R3)
  - Provides electrical isolation for safety
  - Input current: 7-20mA (typical 10mA)
  - Arduino digital pins can easily supply this current
- **Input Signals:**
  - PUL+ / PUL- (Step/Pulse)
  - DIR+ / DIR- (Direction)
  - ENA+ / ENA- (Enable)
- **Pulse Frequency Capability:**
  - 5V mode: Up to 500 KHz (our speeds: <2 KHz)
  - 24V mode: Up to 200 KHz (not used in our setup)

### DIP Switch Configuration

| Switch | Function | Options |
|--------|----------|---------|
| **SW1-SW4** | Resolution/Steps per Rev | 200-51,200 (via combination) |
| **SW5** | Motor Direction | OFF = Clockwise, ON = Counterclockwise |
| **SW6** | Control Mode | OFF = Closed Loop, ON = Open Loop |
| **SW7** | Pulse Type | OFF = Step&Direction, ON = CW&CCW |
| **SW8** | Output Mode | OFF = Position Reach, ON = Brake Output |

### Protection Features

The CL86T includes comprehensive protection:

- **Over-Voltage Protection:** Prevents damage from excessive voltage
- **Over-Current Protection:** Protects motor and driver from excessive current
- **Position Following Error Detection:** Monitors encoder feedback for step loss
- **Automatic Error Correction:** Closed-loop mode compensates for missed steps
- **Thermal Protection:** Prevents overheating

### Closed-Loop Operation

- **No Loss of Step:** Closed-loop mode ensures accurate positioning
- **No Tuning Required:** Automatic compensation
- **Encoder Feedback:** Uses motor encoder for position verification
- **Error Detection:** Detects and corrects position errors automatically

## Detailed Electrical Specifications

### Current Ratings

| Parameter | Min | Typical | Max | Unit | Our Setting |
|----------|-----|---------|-----|------|-------------|
| **Peak Current** | 5.6A (RMS 4.0A) | 7.0A (RMS 5.0A) | 8.0A (RMS 5.7A) | A | 8.0A (RMS 5.7A) - Position A |

### Power Supply

| Parameter | Min | Typical | Max | Unit | Our Setting |
|----------|-----|---------|-----|------|-------------|
| **Operating Voltage** | 18/24 | - | 80/110 | VAC/VDC | 60V DC ✅ |

### Input Signals

| Parameter | Min | Typical | Max | Unit | Notes |
|----------|-----|---------|-----|------|-------|
| **Input Signal Voltage** | 5 | - | 24 | VDC | We use 5V (Arduino) |
| **Logic Input Current** | 7 | 10 | 20 | mA | Optically isolated |
| **Pulse Frequency (5V)** | 0 | - | 500 | kHz | Our speeds: <2 kHz |
| **Pulse Frequency (24V)** | 0 | - | 200 | kHz | Not used (5V mode) |

### Output Signals

| Parameter | Min | Typical | Max | Unit | Notes |
|----------|-----|---------|-----|------|-------|
| **Logic Current Output** | - | - | 100 | mA | For status/control signals |

### Environmental Specifications

| Parameter | Specification | Notes |
|-----------|--------------|-------|
| **Cooling** | Natural or Forced | Ensure adequate ventilation |
| **Operating Temperature** | 0°C - 40°C (32°F - 102°F) | Keep within range |
| **Storage Temperature** | -20°C - 65°C (-4°F - 149°F) | For storage only |
| **Humidity** | 40%RH - 90%RH | Avoid condensation |
| **Vibration** | 10-50Hz / 0.15mm | Normal operation |
| **Environment** | Avoid dust, oil fog, corrosive gases | Clean, dry environment |
| **Weight** | Approx. 600g | Driver weight |

## Technical Specifications Summary

| Parameter | Specification | Our Setting |
|-----------|--------------|-------------|
| **Input Voltage** | 18-80VAC or 24-110VDC | 60V DC ✅ |
| **Max Pulse Frequency (5V)** | 500 KHz | <2 KHz (plenty of headroom) |
| **Max Pulse Frequency (24V)** | 200 KHz | Not used (5V mode) |
| **Resolution Range** | 200-51,200 steps/rev | 200 steps/rev |
| **Current Settings** | 3 ranges via S1 | Position A (5.7A RMS) |
| **Input Logic** | 5V or 24V (optically isolated) | 5V (Arduino) |
| **Input Current** | 7-20mA (typical 10mA) | Within Arduino capability |
| **Control Mode** | Closed/Open loop | Closed Loop (SW6=OFF) |
| **Pulse Type** | Step&Dir or CW&CCW | Step&Dir (SW7=OFF) |
| **Operating Temperature** | 0°C - 40°C | Monitor during operation |
| **Weight** | ~600g | Driver only |

## Advantages for Our Application

### For Adaptive Drum Kit

1. **High Resolution Capability:**
   - Can increase to 51,200 steps/rev for ultra-fine control
   - Currently using 200 for maximum torque

2. **Closed-Loop Reliability:**
   - No step loss ensures accurate drum positioning
   - Critical for musical timing and precision
   - Automatic error correction

3. **High Speed Capability:**
   - 500 KHz max frequency (5V mode) allows for very fast movements
   - Our speeds (100-2000 steps/sec) are well within limits
   - Massive headroom for future high-speed applications

4. **Protection Features:**
   - Over-voltage protection (our 60V is within 110V limit)
   - Over-current protection prevents motor damage
   - Position error detection ensures accuracy
   - Thermal protection for safe operation
   - Operating temperature range: 0-40°C

5. **Flexible Configuration:**
   - Easy to adjust resolution, direction, and mode
   - No software changes needed for basic adjustments
   - Optically isolated inputs (5V or 24V)

6. **Low Input Current:**
   - Only 7-20mA input current (typical 10mA)
   - Arduino digital pins easily supply this
   - No additional current amplification needed

## Configuration for 34E1K-120 Motor

### Recommended Settings

- **SW1-SW4:** All ON (200 steps/rev)
- **SW5:** OFF (Clockwise - can invert in software)
- **SW6:** OFF (Closed Loop - uses encoder)
- **SW7:** OFF (Step&Direction - matches our code)
- **SW8:** OFF (Position Reach output)
- **S1:** Position A (5.7A RMS - maximum for 6.0A motor)

### Why These Settings?

1. **200 Steps/Rev:** Full step mode provides maximum torque
2. **Closed Loop:** Uses built-in encoder for accuracy
3. **Step&Direction:** Matches Arduino code implementation
4. **5.7A RMS:** Maximum driver output, 95% of motor rating (safe)

## Future Expansion Options

### Higher Resolution

If you need finer control:
- Change SW1-SW4 for microstepping (400, 800, 1600, etc.)
- Update `STEPS_PER_REV` in `include/pins.h`
- Trade-off: Lower torque per step, but smoother motion

### Higher Speed

Current code uses 100-2000 steps/sec. Driver supports up to 500,000 steps/sec:
- Can increase `SPEED_MAX_SPS` in `pins.h`
- Ensure motor can handle higher speeds
- May need to adjust acceleration

### Position Feedback

The CL86T can provide position reach signals:
- Configure via SW8
- Can connect to Arduino for position confirmation
- Useful for limit switches or position verification

## Safety Features

### Built-in Protections

1. **Over-Voltage Protection:**
   - Protects driver from excessive input voltage
   - Our 60V supply is well within 110V DC limit ✅
   - Automatic shutdown if voltage exceeds maximum

2. **Over-Current Protection:**
   - Prevents damage from excessive current
   - Current limited by S1 switch setting (max 5.7A RMS)
   - Automatic protection if current exceeds limits

3. **Position Error Detection:**
   - Detects when motor position doesn't match commands
   - Automatically corrects in closed-loop mode
   - Prevents step loss accumulation

4. **Thermal Protection:**
   - Prevents overheating
   - Monitor driver temperature during operation
   - Ensure operating temperature stays within 0-40°C range

### Environmental Considerations

- **Temperature:** Keep operating environment between 0-40°C (32-102°F)
- **Humidity:** Maintain 40-90%RH, avoid condensation
- **Ventilation:** Ensure adequate airflow for cooling
- **Clean Environment:** Avoid dust, oil fog, and corrosive gases
- **Mounting:** Use non-conductive mounting hardware, allow space for heat dissipation

### Heat Dissipation Guidelines

**Critical Thermal Management:**

1. **Maximum Reliable Working Temperature:**
   - **40°C (109°F)** - Driver should not exceed this temperature
   - Monitor driver temperature during operation
   - If temperature approaches limit, reduce current or add cooling

2. **Mounting Orientation:**
   - **Mount vertically** for maximum heat dissipation
   - Vertical mounting allows natural convection to work best
   - Heat rises, so vertical orientation maximizes airflow

3. **Cooling Fan:**
   - **Recommended:** Mount a cooling fan nearby if needed
   - Use if operating near maximum current (5.7A RMS)
   - Use if ambient temperature is high
   - Use if driver runs continuously for extended periods
   - Fan should blow air across the driver heat sink

4. **Multiple Drivers:**
   - If installing multiple CL86T drivers:
   - **Minimum spacing:** 30mm (12 inches) between drivers
   - Prevents heat buildup between units
   - Ensures adequate airflow around each driver

5. **Heat Dissipation Best Practices:**
   - Mount driver vertically (heat sink fins vertical)
   - Ensure adequate clearance around driver (at least 50mm recommended)
   - Use in well-ventilated area
   - Monitor temperature during initial operation
   - Add cooling fan if temperature exceeds 35°C during operation
   - Avoid mounting in enclosed spaces without ventilation

## Troubleshooting

### Driver Not Responding

- Check input voltage (should be 24-110V DC)
- Verify power connections
- Check DIP switch settings

### Position Errors

- Ensure closed-loop mode is enabled (SW6=OFF)
- Verify encoder connections
- Check encoder power supply (5V)

### Over-Current Protection

- Reduce S1 current setting
- Check for mechanical binding
- Verify motor connections

## Additional Resources

- **CL86T Manual:** See `CL86T (V4.0) manual.pdf` for complete specifications
- **DIP Switch Settings:** See [DIP Switch Settings Guide](dip-switch-settings.md)
- **Current Settings:** See [Current Settings Guide](current-settings.md)
- **Wiring Diagram:** See [Wiring Diagram](wiring-diagram.md)

