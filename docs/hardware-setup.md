# Hardware Setup Guide

## Overview

This guide walks you through the complete hardware setup process for the adaptive drum kit using the CL86T closed-loop stepper driver system.

## Prerequisites

Before starting, ensure you have:

- [ ] All hardware components (motor, driver, power supply, Arduino)
- [ ] Appropriate wire (14-16 AWG for power, 22-24 AWG for signals)
- [ ] Wire strippers and crimping tools
- [ ] Multimeter for testing
- [ ] Safety equipment (safety glasses, insulated tools)
- [ ] Well-lit, clean workspace

## Safety First

**⚠️ IMPORTANT SAFETY WARNINGS:**

1. **High Voltage:** The LE-350-60 power supply outputs 60V DC, which can be dangerous
2. **Always disconnect power** before making or changing connections
3. **Double-check all connections** before applying power
4. **Use proper wire gauge** for current capacity
5. **Ensure proper grounding** of all components
6. **Keep work area dry** and free of conductive materials
7. **Never touch live connections** while power is applied

## Step-by-Step Setup

### Step 1: Prepare the Workspace

1. Clear a clean, well-lit workspace
2. Lay out all components
3. Ensure adequate ventilation (power supply generates heat)
4. Have multimeter and tools ready

### Step 2: Power Supply Setup

#### 2.1 Inspect Power Supply
- Check LE-350-60 for physical damage
- **Input Voltage:** Verify matches your AC supply (115/230V AC ±20%)
- **Input Frequency:** 47-63 Hz
- **Output:** Ensure set to 60V DC (adjustable ±10%: 54V-66V)
- **Cooling:** Verify fan is free to rotate (fan cooling)
- **Mounting:** Can be mounted vertically or horizontally
- **Dimensions:** 215×115×30mm (8.5"×4.5"×1.2")

#### 2.2 Connect AC Input

**LE-350-60 AC Input Terminals:**
- **L** (line/live) - Connect to hot wire
- **N** (neutral) - Connect to neutral wire
- **GND** (ground) - Connect to ground wire (safety)

**AC Input Wire Colors:**
- **Blue** → **L** (line/live)
- **Brown** → **N** (neutral)
- **Yellow** → **GND** (ground)

**Connection Steps:**
1. Connect **Blue** wire to **L** terminal (line/live)
2. Connect **Brown** wire to **N** terminal (neutral)
3. Connect **Yellow** wire to **GND** terminal (safety ground)
4. **DO NOT** plug into wall outlet yet
5. Verify input voltage matches supply (115/230V AC ±20%)
6. Ensure proper grounding

#### 2.3 Prepare DC Output Wires
- Cut appropriate length of 14-16 AWG wire for VCC and GND
- Strip ends (about 5mm)
- Use red wire for VCC (+), black for GND (-)
- Leave unconnected for now

#### 2.4 EMI Filter (Optional but Recommended)

**⚠️ Recommended for Noise Immunity:**

If available, install EMI line filter between power supply and driver:

1. **Select EMI Filter:**
   - Rated for 60V DC
   - Rated for 5.8A minimum
   - DC-rated filter

2. **Installation:**
   - Connect power supply output to filter input
   - Connect filter output to CL86T VCC/GND
   - Install as close to driver as possible

3. **Benefits:**
   - Reduces electrical noise
   - Improves reliability in interference environments
   - Prevents false triggering

**Note:** Not strictly required, but recommended for best performance, especially in noisy environments or with multiple drivers.

### Step 3: CL86T Driver Setup

#### 3.1 Inspect Driver
- Check CL86T for physical damage
- Verify all terminals are clean and undamaged
- Review terminal labels (VCC, GND, PUL+, PUL-, DIR+, DIR-, ENA+, ENA-, A+, A-, B+, B-)

#### 3.2 Configure Driver Voltage Setting

**⚠️ IMPORTANT: Set 5V/24V Rotating Switch**

The CL86T factory default is 24V input signals. Since we're using Arduino (5V), you **must** set the rotating switch to **5V**:

1. Locate the 5V/24V rotating switch on the CL86T driver
2. Set it to **5V** position (for Arduino compatibility)
3. Verify the setting before proceeding

**If this switch is not set correctly:**
- Arduino signals may not be recognized
- Driver may not respond to commands
- Motor will not move

#### 3.3 Configure Driver DIP Switches

**Recommended DIP Switch Settings:**

| Switch | Position | Setting                    |
| ------ | -------- | -------------------------- |
| SW1    | **ON**   | Pulse/Rev                  |
| SW2    | **ON**   | Pulse/Rev                  |
| SW3    | **ON**   | Pulse/Rev                  |
| SW4    | **ON**   | Pulse/Rev                  |
| SW5    | **ON**   | Direction (Clockwise)      |
| SW6    | **OFF**  | Control Mode (Closed Loop) |
| SW7    | **OFF**  | Pulse Mode (Pul/Dir)       |
| SW8    | **OFF**  | Mode (Brake Output)        |

**Result:** 200 pulse/revolution, Clockwise, Closed Loop, Pul/Dir mode, Brake mode

**Detailed explanation:** See [DIP Switch Settings Guide](dip-switch-settings.md)

#### 3.3 Configure Current Setting (S1 Rotary Switch)

**Recommended S1 Setting for 34E1K-120 (6.0A rated):**

| Switch Position | Peak Current | RMS Current | Gains (Ki/Kp/Kp) | Recommendation                           |
| --------------- | ------------ | ----------- | ---------------- | ---------------------------------------- |
| **A**           | 8.0A         | 5.7A        | 0/25/25          | ⭐ **Recommended start** (smooth, stable) |
| **B**           | 8.0A         | 5.7A        | 0/50/15          | Higher position gain (faster response)   |
| **C**           | 8.0A         | 5.7A        | 0/100/5          | Maximum position gain (fastest response) |
| **D-F**         | 8.0A         | 5.7A        | 16/var/var       | With velocity integral (better tracking) |
| **4** (or 4-9)  | 7.0A         | 5.0A        | Various          | Alternative if motor runs too hot        |

**How to Set:**
1. Power OFF the system
2. Locate S1 rotary switch on CL86T driver
3. Rotate to position **A** (or any position A-F for 5.7A RMS)
4. Verify position visually
5. Power ON and test

**Important Notes:**
- **34E1K-120 is rated at 6.0A**, but CL86T maximum is 5.7A RMS
- Position A-F provides 5.7A RMS (95% of motor rating) - safe and recommended
- This is the maximum driver setting and appropriate for this motor
- Monitor motor temperature during initial testing
- If motor runs too hot, reduce to position 4 (5.0A RMS)

**Complete S1 settings guide:** See [Current Settings Guide](current-settings.md)

**Additional Configuration:**
- Configure closed-loop parameters (if using encoder feedback)

#### 3.4 Mount Driver

**Mounting Orientation:**
- **Mount CL86T vertically** (heat sink fins vertical) for maximum heat dissipation
- Vertical mounting allows natural convection to work best
- Heat rises, so vertical orientation maximizes airflow

**Location and Spacing:**
- Mount in well-ventilated location
- Ensure adequate clearance around driver (at least 50mm recommended)
- If using multiple CL86T drivers, keep **minimum 30mm (12 inches)** spacing between them
- Use non-conductive mounting hardware

**Cooling Considerations:**
- **Maximum reliable working temperature: 40°C (109°F)**
- Monitor driver temperature during operation
- **Add cooling fan nearby if:**
  - Operating at maximum current (5.7A RMS)
  - Ambient temperature is high
  - Driver runs continuously for extended periods
  - Temperature approaches 35°C during operation

**Environmental Requirements:**
- Operating temperature: 0-40°C (32-102°F)
- Humidity: 40-90%RH (avoid condensation)
- Avoid dust, oil fog, and corrosive gases
- Ensure adequate airflow for cooling (natural or forced)

### Step 4: Check LED Indicators

After powering on the CL86T driver:

1. **GREEN LED:**
   - Should be **ON** when driver is powered
   - Indicates power is connected correctly
   - If OFF, check power connections

2. **RED LED:**
   - Should be **OFF** during normal operation
   - If ON or flashing, indicates alarm condition
   - Check connections, voltage, and current settings

**If RED LED is flashing:**
- Check power supply voltage (should be 24-110VDC)
- Check current setting (may be too high)
- Check motor connections
- Check encoder connections (if using closed-loop)
- Refer to troubleshooting guide

### Step 5: Motor Preparation

#### 4.1 Identify Motor Power Wires

The 34E1K-120 has **4 thick wires** in one cable bundle. Colors: **Red, Green, Blue, Black**

**✅ Verified Configuration:**
- **A+** → **Black**
- **A-** → **Green**
- **B+** → **Red**
- **B-** → **Blue**

**If you need to verify (multimeter method):**
1. **Power OFF** the system
2. Set multimeter to resistance/continuity mode
3. Test pairs of wires - pairs will show continuity (~0.72Ω)
4. One pair = Phase A (A+ and A-)
5. Other pair = Phase B (B+ and B-)
6. Between different phases: High resistance (open circuit)

**For detailed wire identification, see [Motor Wire Identification Guide](motor-wire-identification.md)**

#### 4.2 Identify Encoder Wires

The 34E1K-120 has **encoder wires** in a connector. Colors: **Black (thin), Red, Green, White, Yellow, Blue, Black (thick)**

**Standard Encoder Wire Colors:**
- **Red:** VCC (+5V power - driver supplies)
- **Black (thin):** EGND (ground)
- **Green:** EA+ (encoder A positive)
- **White:** EA- (encoder A negative)
- **Yellow:** EB+ (encoder B positive)
- **Blue:** EB- (encoder B negative)
- **Black (thick):** Shield/ground (optional)

**For detailed wire identification, see [Motor Wire Identification Guide](motor-wire-identification.md)**

#### 4.3 Prepare Motor Wires
- Strip wire ends (about 5mm) - power wires only (encoder has connector)
- Ensure no frayed strands
- Label wires if needed (A+, A-, B+, B-)

### Step 6: Wiring Connections

#### 5.1 Power Supply to Driver

**LE-350-60 DC Output Terminals:**
- **+V, +V, +V** (three positive terminals - use any one)
- **-V, -V, -V** (three negative terminals - use any one)
- **GND** (ground/frame ground - optional)

**CL86T Power Input Terminals:**
- **AC, AC** (two AC terminals - accept AC or DC input, no polarity for DC)

**Connection Steps:**
1. **Disconnect power supply from AC** (if connected)
2. Connect **+V** wire (red, from any +V terminal) to CL86T **AC** (first terminal)
3. Connect **-V** wire (black, from any -V terminal) to CL86T **AC** (second terminal)
4. **Optional:** Connect **GND** to frame ground for safety
5. Ensure connections are tight and secure
6. **Note:** CL86T AC terminals accept DC input - no polarity requirement for DC
7. **Double-check connections** - verify with multimeter before powering on

#### 5.2 Motor Power Wires to Driver

**✅ Verified Configuration:**
1. Connect **Black** wire to CL86T **A+** terminal
2. Connect **Green** wire to CL86T **A-** terminal
3. Connect **Red** wire to CL86T **B+** terminal
4. Connect **Blue** wire to CL86T **B-** terminal
5. Ensure all connections are tight
6. Verify no loose strands or shorts

**Note:** If motor vibrates but doesn't rotate, swap A+ and A- (Black ↔ Green) or B+ and B- (Red ↔ Blue)

#### 5.2a Encoder Wires to Driver

**CL86T Encoder Terminals:**
- **EA+, EA-** (Encoder channel A differential input)
- **EB+, EB-** (Encoder channel B differential input)
- **VCC, EGND** (Encoder power - VCC supplies 5V OUTPUT, EGND is ground)

**Connection Steps:**
1. **Red** → CL86T **VCC** (encoder power - driver supplies 5V)
2. **Black (thin)** → CL86T **EGND** (encoder ground)
3. **Green** → CL86T **EA+** (encoder A positive)
4. **White** → CL86T **EA-** (encoder A negative)
5. **Yellow** → CL86T **EB+** (encoder B positive)
6. **Blue** → CL86T **EB-** (encoder B negative)
7. **Black (thick)** → CL86T **EGND** or separate ground (shield, optional)

**⚠️ Important:** CL86T VCC supplies 5V to encoder - do not connect external 5V power

#### 5.3 Arduino to Driver (Signal Connections)

**CL86T Control Signal Terminals:**
- **PUL+, PUL-** (Pulse/Step signal)
- **DIR+, DIR-** (Direction signal)
- **ENA+, ENA-** (Enable signal, optional)

**Connection Steps:**
1. Connect Arduino Digital Pin 2 to CL86T **PUL+** terminal
2. Connect Arduino Digital Pin 3 to CL86T **DIR+** terminal
3. Connect Arduino Digital Pin 4 to CL86T **ENA+** terminal (optional - no connection is default)
4. Connect Arduino GND to CL86T **PUL-** terminal
5. Connect Arduino GND to CL86T **DIR-** terminal
6. Connect Arduino GND to CL86T **ENA-** terminal

**Note:** You can use a single GND wire with multiple connections, or separate wires - both methods work.

**Enable Signal (ENA):**
- **Default:** No connection (driver enabled by default)
- **Optional:** Connect for enable/disable control
- **Voltage:** 5V-24V supported
- **If used:** Must advance DIR signal by minimum 200ms

#### 5.4 Optional: Fault Output Connection (ALM+/ALM-)

**CL86T Alarm Terminal:**
- **ALM+, ALM-** (Alarm/Fault output, max 30VDC/100mA)

**For monitoring driver status:**

1. **Connection Options:**
   - Can connect to Arduino digital input (with voltage level conversion if needed)
   - Can connect to PLC or other control system
   - Sinking or sourcing connection supported

2. **Function:**
   - Changes state when fault occurs (over-voltage, over-current, etc.)
   - RED LED also provides visual indication
   - Optional - driver works without this connection

3. **Use Cases:**
   - Remote fault monitoring
   - Safety shutdown triggers
   - Fault logging

#### 5.5 Optional: Brake Output Connection (BRK/PEND+/BRK/PEND-)

**CL86T Brake/Pend Terminal:**
- **BRK/PEND+, BRK/PEND-** (Brake/Pend output, max 30VDC/100mA)

**For motor brake control (if motor has brake):**

1. **Requires Relay:**
   - Brake output cannot directly drive motor brake
   - Must use relay between BRK/PEND+ and BRK/PEND- and brake coil
   - Select relay appropriate for brake voltage/current

2. **Connection:**
   - CL86T BRK/PEND+ → Relay coil
   - CL86T BRK/PEND- → Relay coil
   - Relay contacts → Motor brake coil
   - Power supply → Motor brake coil

3. **SW8 Setting:**
   - Set SW8 to OFF for brake output mode
   - Set SW8 to ON for pend output mode

4. **Use Cases:**
   - Motor holding brake control
   - Safety brake activation
   - Controlled brake release

**Note:** Only needed if motor has brake and you want automatic control.

#### 5.4 Optional: Resistor Protection
If adding 1kΩ resistors for extra protection:
1. Insert 1kΩ resistor between Arduino Pin 2 and PUL+
2. Insert 1kΩ resistor between Arduino Pin 3 and DIR+
3. Insert 1kΩ resistor between Arduino Pin 4 and ENA+

### Step 7: Pre-Power Checks

Before applying power, perform these checks:

#### 6.1 Visual Inspection
- [ ] All connections are secure
- [ ] No loose wires or strands
- [ ] No exposed conductors
- [ ] Proper wire gauge used
- [ ] Motor shaft can rotate freely

#### 6.2 Continuity Tests (Power OFF)
Using multimeter:

1. **Power Supply Output:**
   - Measure resistance between VCC and GND (should be high, not shorted)
   - Verify voltage setting (if adjustable)

2. **Motor Phases:**
   - Measure resistance between A+ and A- (should be 0.5-2.0 ohms)
   - Measure resistance between B+ and B- (should be 0.5-2.0 ohms)
   - Measure resistance between A+ and B+ (should be high/open)

3. **Signal Connections:**
   - Verify Arduino pins are not shorted to GND or VCC
   - Check continuity of signal wires

#### 6.3 Arduino Check
- [ ] Arduino is powered via USB
- [ ] Arduino LED is on
- [ ] No error messages in Arduino IDE/PlatformIO
- [ ] Code is uploaded successfully

### Step 8: Initial Power-On

#### 7.1 Power Sequence
1. **Ensure motor shaft is free to rotate**
2. Connect Arduino USB to computer
3. Open serial monitor (115200 baud)
4. **Plug in power supply AC input**
5. Observe power supply LED/indicator
6. Check for any unusual sounds or smells

#### 7.2 Initial Test
1. Send '?' command via serial monitor
2. Verify Arduino responds with status
3. Send 'R' command to rotate one revolution
4. Observe motor movement

### Step 9: Verification and Calibration

#### 8.1 Basic Movement Test
- Test forward movement (F command)
- Test backward movement (B command)
- Test stop (S command)
- Test home position (H command)

#### 8.2 Direction Verification
- If motor moves opposite to expected, change `DIR_INVERT` in `pins.h`
- Or swap DIR+ and DIR- connections

#### 8.3 Speed Calibration
- Test different speeds using '+' and '-' commands
- Find optimal speed for your application
- Note any vibration or resonance issues

#### 8.4 Position Accuracy
- Test step accuracy (N command with known step count)
- Test degree accuracy (A command with known angles)
- Verify motor returns to home position accurately

## Troubleshooting

### Motor Doesn't Move

**Check:**
1. Power supply is on and outputting voltage
2. CL86T power LED is on (if present)
3. Motor enable signal is active
4. Motor wires are connected correctly
5. Arduino is sending step pulses (check with oscilloscope if available)

**Solution:**
- Swap motor phase wires (A+ with A- or B+ with B-)
- Check enable pin configuration
- Verify step pulse frequency is within driver range

### Motor Vibrates but Doesn't Rotate

**Cause:** Incorrect motor phase wiring

**Solution:**
- Swap A+ and A- wires
- Or swap B+ and B- wires
- Or swap both pairs

### Motor Moves in Wrong Direction

**Solution:**
- Change `DIR_INVERT` to `true` in `pins.h`
- Or swap DIR+ and DIR- connections

### Motor Moves Erratically

**Check:**
1. Loose connections
2. Interference on signal wires
3. Power supply voltage stability
4. Driver current settings

**Solution:**
- Tighten all connections
- Use shielded/twisted pair cables for signals
- Check power supply output with multimeter
- Review driver current limit settings

### Arduino Not Responding

**Check:**
1. USB cable connection
2. Correct COM port selected
3. Serial monitor baud rate (115200)
4. Arduino is powered (LED on)

**Solution:**
- Try different USB cable
- Check Device Manager for COM port
- Restart Arduino IDE/PlatformIO
- Re-upload code

## Maintenance

### Regular Checks
- Inspect connections monthly for looseness or corrosion
- Check wire insulation for damage
- Verify motor runs smoothly
- Monitor power supply temperature

### Cleaning
- Keep components free of dust
- Clean terminals if corrosion appears
- Ensure adequate ventilation

### Storage
- Disconnect power when not in use
- Store in dry, temperature-controlled environment
- Protect connections from moisture

## Next Steps

Once hardware is verified working:
1. Proceed to software development
2. Implement closed-loop feedback (if using encoder)
3. Add additional sensors and controls
4. Integrate with drum mechanism

## Additional Resources

- [Wiring Diagram](wiring-diagram.md) - Detailed connection diagrams
- [CL86T Specifications](cl86t-specifications.md) - Complete CL86T driver specifications
- [DIP Switch Settings](dip-switch-settings.md) - Complete DIP switch configuration guide
- [Current Settings](current-settings.md) - S1 rotary switch current configuration
- [Power Supply Selection](power-supply-selection.md) - Power supply selection and configuration
- [Troubleshooting Guide](troubleshooting.md) - Common issues and solutions
- CL86T Manual - Driver-specific configuration
- 34E1K-120 Datasheet - Motor specifications
- LE-350-60 Datasheet - Power supply specifications

