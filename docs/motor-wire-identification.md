# Motor Wire Identification and Connection Guide

## Overview

The 34E1K-120 motor has two cable bundles:
1. **Power Cable Bundle:** 4 thick wires (motor phases)
2. **Encoder Cable Bundle:** Connector with 7 wires (encoder signals)

## Power Cable Bundle (4 Wires)

### Wire Colors
- **Red**
- **Green**
- **Blue**
- **Black**

### Actual Configuration (34E1K-120 Motor)

**✅ Verified Wiring:**
- **A+** → **Black**
- **A-** → **Green**
- **B+** → **Red**
- **B-** → **Blue**

This is the actual wire configuration for the 34E1K-120 motor in this project.

### Identification Method (If Needed)

**Method 1: Multimeter Resistance Test**

1. **Power OFF** the system
2. Set multimeter to resistance/continuity mode
3. Test pairs of wires - pairs will show continuity (~0.72Ω)

**Expected Results:**
- One pair will show ~0.72Ω resistance → This is Phase A (A+ and A-)
- Another pair will show ~0.72Ω resistance → This is Phase B (B+ and B-)
- Between different phases: High resistance (open circuit)

**Verified Pairing:**
- **Phase A:** Black (A+) and Green (A-)
- **Phase B:** Red (B+) and Blue (B-)

**Note:** The + and - designation doesn't matter initially - you can swap them if needed. If motor vibrates, swap A+ and A- (or B+ and B-).

### Connection to CL86T

**✅ Verified Configuration:**

| Motor Wire Color | CL86T Terminal | Function         | Notes      |
| ---------------- | -------------- | ---------------- | ---------- |
| **Black**        | A+             | Phase A positive | ✅ Verified |
| **Green**        | A-             | Phase A negative | ✅ Verified |
| **Red**          | B+             | Phase B positive | ✅ Verified |
| **Blue**         | B-             | Phase B negative | ✅ Verified |

**Connection Steps:**
1. Connect **Black** wire to CL86T **A+** terminal
2. Connect **Green** wire to CL86T **A-** terminal
3. Connect **Red** wire to CL86T **B+** terminal
4. Connect **Blue** wire to CL86T **B-** terminal

**Important:** If motor vibrates but doesn't rotate, swap A+ and A- (Black ↔ Green) or B+ and B- (Red ↔ Blue).

## Encoder Cable Bundle (7 Wires)

### Wire Colors in Connector
- **Black** (thinner)
- **Red**
- **Green**
- **White**
- **Yellow**
- **Blue**
- **Black** (thicker - likely shield/ground)

### Standard Encoder Wire Color Codes

**Typical 1000 PPR Encoder Wiring:**

| Wire Color          | Function         | CL86T Terminal   | Pin Number                      |
| ------------------- | ---------------- | ---------------- | ------------------------------- |
| **Red**             | VCC (+5V)        | VCC              | Encoder power (driver supplies) |
| **Black** (thinner) | EGND (Ground)    | EGND             | Encoder ground                  |
| **Green**           | EA+ (Encoder A+) | EA+              | Encoder channel A positive      |
| **White**           | EA- (Encoder A-) | EA-              | Encoder channel A negative      |
| **Yellow**          | EB+ (Encoder B+) | EB+              | Encoder channel B positive      |
| **Blue**            | EB- (Encoder B-) | EB-              | Encoder channel B negative      |
| **Black** (thicker) | Shield/Ground    | EGND or separate | Optional shield connection      |

### Verification Method

**If colors don't match standard, use continuity/resistance test:**

1. **Power OFF** the system
2. **Disconnect encoder connector** (if possible)
3. Set multimeter to resistance mode

**Expected Results:**
- **Power wires (VCC/EGND):** May show continuity to encoder board
- **Channel A (EA+/EA-):** Differential pair, may show some resistance
- **Channel B (EB+/EB-):** Differential pair, may show some resistance
- **Shield:** Thicker black wire, typically connected to ground

### Connection to CL86T

**Based on Standard Color Code:**

| Encoder Wire        | CL86T Terminal   | Notes                                             |
| ------------------- | ---------------- | ------------------------------------------------- |
| **Red**             | VCC              | ⚠️ Driver supplies 5V - do not connect external 5V |
| **Black** (thinner) | EGND             | Encoder ground                                    |
| **Green**           | EA+              | Encoder channel A positive                        |
| **White**           | EA-              | Encoder channel A negative                        |
| **Yellow**          | EB+              | Encoder channel B positive                        |
| **Blue**            | EB-              | Encoder channel B negative                        |
| **Black** (thicker) | EGND or separate | Shield/ground (optional)                          |

**⚠️ Important:** The CL86T VCC terminal **supplies** 5V to the encoder. Do not connect external 5V power.

## Step-by-Step Connection Process

### Step 1: Identify Motor Power Wires

1. **Power OFF** everything
2. Use multimeter to find two pairs with ~0.72Ω resistance
3. Label one pair as Phase A (A+ and A-)
4. Label other pair as Phase B (B+ and B-)

### Step 2: Connect Motor Power Wires

1. Connect Phase A wires to CL86T A+ and A- terminals
2. Connect Phase B wires to CL86T B+ and B- terminals
3. **Note:** If motor vibrates, swap A+ and A- (or B+ and B-)

### Step 3: Identify Encoder Wires

**If connector matches standard:**
- Red = VCC (5V power)
- Black (thin) = EGND (ground)
- Green = EA+
- White = EA-
- Yellow = EB+
- Blue = EB-
- Black (thick) = Shield

**If unsure, check with multimeter:**
- Power wires typically have continuity to encoder board
- Signal wires are differential pairs

### Step 4: Connect Encoder Wires

1. **Red** → CL86T **VCC** (encoder power - driver supplies 5V)
2. **Black** (thin) → CL86T **EGND** (encoder ground)
3. **Green** → CL86T **EA+** (encoder A positive)
4. **White** → CL86T **EA-** (encoder A negative)
5. **Yellow** → CL86T **EB+** (encoder B positive)
6. **Blue** → CL86T **EB-** (encoder B negative)
7. **Black** (thick) → CL86T **EGND** or separate ground (shield)

## Troubleshooting

### Motor Doesn't Move

**Check:**
1. Power wires connected correctly (A+, A-, B+, B-)
2. Power supply is on
3. Driver enable signal is active

**If motor vibrates:**
- Swap A+ and A- wires
- Or swap B+ and B- wires
- Or swap both pairs

### Encoder Not Working (Closed-Loop Issues)

**Check:**
1. VCC connected to CL86T VCC (driver supplies 5V)
2. EGND connected to CL86T EGND
3. EA+/EA- connected correctly (differential pair)
4. EB+/EB- connected correctly (differential pair)
5. RED LED not flashing (no driver faults)

**If encoder colors don't match:**
- Use multimeter to identify power wires (VCC/EGND)
- Identify differential pairs (EA+/EA- and EB+/EB-)
- Refer to encoder datasheet if available

### Wrong Direction

**Solutions:**
1. Swap A+ and A- wires
2. Or swap B+ and B- wires
3. Or change SW5 DIP switch (ON = CW, OFF = CCW)
4. Or change `DIR_INVERT` in `pins.h`

## Wire Color Reference Table

### Motor Power Wires (4 wires)

**✅ Verified Configuration for 34E1K-120:**

| Color     | Function | CL86T Terminal | Notes      |
| --------- | -------- | -------------- | ---------- |
| **Black** | Phase A+ | A+             | ✅ Verified |
| **Green** | Phase A- | A-             | ✅ Verified |
| **Red**   | Phase B+ | B+             | ✅ Verified |
| **Blue**  | Phase B- | B-             | ✅ Verified |

**If colors differ from above, use multimeter to identify pairs:**
- One pair with ~0.72Ω = Phase A
- Other pair with ~0.72Ω = Phase B

### Encoder Wires (7 wires)

| Color         | Typical Function | CL86T Terminal | Notes                 |
| ------------- | ---------------- | -------------- | --------------------- |
| Red           | VCC (+5V)        | VCC            | Driver supplies power |
| Black (thin)  | EGND (Ground)    | EGND           | Encoder ground        |
| Green         | EA+ (Encoder A+) | EA+            | Channel A positive    |
| White         | EA- (Encoder A-) | EA-            | Channel A negative    |
| Yellow        | EB+ (Encoder B+) | EB+            | Channel B positive    |
| Blue          | EB- (Encoder B-) | EB-            | Channel B negative    |
| Black (thick) | Shield/Ground    | EGND           | Optional shield       |

## Safety Reminders

⚠️ **Always power OFF before making connections**
⚠️ **Double-check wire identification with multimeter**
⚠️ **Verify encoder VCC is connected to driver VCC (not external 5V)**
⚠️ **Test motor without load first**
⚠️ **Monitor for overheating during initial testing**

## Additional Resources

- **Wiring Diagram:** See [Wiring Diagram](wiring-diagram.md) for complete connection details
- **Hardware Setup:** See [Hardware Setup Guide](hardware-setup.md) for step-by-step instructions
- **Troubleshooting:** See [Troubleshooting Guide](troubleshooting.md) for common issues
- **Motor Datasheet:** See 34E1K-120 datasheet for official wire color codes

