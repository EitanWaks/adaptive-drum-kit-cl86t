# CL86T Current Settings (S1 Rotary Switch)

## Overview

The S1 rotary switch on the CL86T driver controls the peak current supplied to the stepper motor. This setting must match your motor's rated current to ensure proper operation and prevent damage.

## S1 Rotary Switch Positions

The S1 switch has 16 positions: **0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F**

## Current Settings Table

| Switch Position      | Peak Current | RMS Current | Code Range | Recommended For               |
| -------------------- | ------------ | ----------- | ---------- | ----------------------------- |
| **0, 1, 2, 3**       | 5.6A         | 4.0A        | 0-3        | Motors rated 4.0A RMS or less |
| **4, 5, 6, 7, 8, 9** | 7.0A         | 5.0A        | 4-9        | Motors rated 5.0A RMS         |
| **A, B, C, D, E, F** | 8.0A         | 5.7A        | A-F        | Motors rated 5.7A RMS         |

## Detailed S1 Switch Settings with Gain Parameters

The S1 rotary switch not only sets current but also configures motion control gains (closed-loop parameters):

| Code  | Peak Current | Velocity Loop Ki | Position Loop Kp | Velocity Loop Kp | Notes                   |
| ----- | ------------ | ---------------- | ---------------- | ---------------- | ----------------------- |
| **0** | 5.6A         | 0                | 25               | 25               | Low gain, smooth        |
| **1** | 5.6A         | 0                | 50               | 15               | Medium position gain    |
| **2** | 5.6A         | 16               | 25               | 25               | With velocity integral  |
| **3** | 5.6A         | 16               | 50               | 15               | Higher gains            |
| **4** | 7.0A         | 0                | 25               | 25               | Low gain, smooth        |
| **5** | 7.0A         | 0                | 50               | 15               | Medium position gain    |
| **6** | 7.0A         | 0                | 100              | 5                | High position gain      |
| **7** | 7.0A         | 16               | 25               | 25               | With velocity integral  |
| **8** | 7.0A         | 16               | 50               | 15               | Higher gains            |
| **9** | 7.0A         | 16               | 100              | 5                | Maximum gains           |
| **A** | 8.0A         | 0                | 25               | 25               | ⭐ **Recommended start** |
| **B** | 8.0A         | 0                | 50               | 15               | Higher position gain    |
| **C** | 8.0A         | 0                | 100              | 5                | Maximum position gain   |
| **D** | 8.0A         | 16               | 25               | 25               | With velocity integral  |
| **E** | 8.0A         | 16               | 50               | 15               | Higher gains            |
| **F** | 8.0A         | 16               | 100              | 5                | Maximum gains           |

**Gain Parameters Explained:**
- **Velocity Loop Ki:** Integral gain for velocity control (0 or 16)
- **Position Loop Kp:** Proportional gain for position control (25, 50, or 100)
- **Velocity Loop Kp:** Proportional gain for velocity control (5, 15, or 25)

**Gain Selection:**
- **Lower gains (A, D):** Smoother motion, less aggressive response
- **Medium gains (B, E):** Balanced response
- **Higher gains (C, F):** Faster response, may be more aggressive

## How to Select the Correct Setting

### Step 1: Check Motor Datasheet

1. **Find the rated current** in your 34E1K-120 motor datasheet
2. Look for:
   - **RMS Current** (Root Mean Square) - This is the continuous current rating
   - **Peak Current** - Maximum current the motor can handle
3. **Note the values** - You'll need the RMS current rating

### Step 2: Match RMS Current

Select the S1 position that provides an **RMS current equal to or slightly below** your motor's rated RMS current:

- **If motor rated ≤ 4.0A RMS**: Use positions **0, 1, 2, or 3** (4.0A RMS)
- **If motor rated = 5.0A RMS**: Use positions **4, 5, 6, 7, 8, or 9** (5.0A RMS)
- **If motor rated = 5.7A RMS**: Use positions **A, B, C, D, E, or F** (5.7A RMS)

### Step 3: Choose Specific Position

Within each range, the position doesn't matter for current (they're all the same). However:
- **Start with the lowest position** in the range (0, 4, or A) for conservative operation
- You can use any position within the range - they provide the same current

## Recommended Settings

### For 34E1K-120 Motor

**Motor Specifications:**
- **Rated Current/Phase:** 6.0A
- **Phase Resistance:** 0.72Ω
- **Inductance:** 7.3mH ± 20%
- **Holding Torque:** 12Nm (1699.34 oz.in)

#### Recommended Setting for 34E1K-120 ⭐

- **Position: A** (recommended start)
- **Peak Current: 8.0A**
- **RMS Current: 5.7A**
- **Gains:** Velocity Ki=0, Position Kp=25, Velocity Kp=25 (low gain, smooth)
- **Why:**
  - Motor is rated at 6.0A, but CL86T maximum is 5.7A RMS
  - Running at 5.7A (95% of rated) is safe and provides excellent performance
  - Slightly below rated current prevents overheating
  - Within power supply limit (5.8A)
  - Low gain settings provide smooth, stable motion

**Alternative Positions (if needed):**
- **Position B:** Same current, higher position gain (Kp=50) - faster response
- **Position C:** Same current, maximum position gain (Kp=100) - fastest response
- **Position D-F:** Same current, with velocity integral (Ki=16) - better tracking
- **Note:** This is the maximum CL86T setting and is appropriate for this motor

#### Alternative Settings (If Needed)

**If motor runs too hot or you want to reduce power consumption:**
- **Position: 4** (or any 4-9)
- **RMS Current: 5.0A**
- **Why:** Lower current reduces heat and power consumption
- **Trade-off:** Slightly less torque (83% of rated)

**For lower-rated motors (≤4.0A RMS):**
- **Position: 0** (or any 0-3)
- **RMS Current: 4.0A**
- **Why:** Matches lower-rated motors

### ⚠️ Important Notes

1. **Motor rated at 6.0A, driver max is 5.7A RMS:**
   - The 34E1K-120 is rated at 6.0A, but CL86T maximum is 5.7A RMS
   - Running at 5.7A (95% of rated) is safe and recommended
   - This provides excellent performance while staying within driver limits
   - Slightly below rated current helps prevent overheating

2. **Never exceed driver rating:** Setting current higher than driver can provide will:
   - Not work (driver limits current)
   - May cause driver overheating
   - May damage driver

3. **Power supply limit:** LE-350-60 provides 5.8A max
   - Position A-F (5.7A RMS) is at the limit
   - Monitor power supply temperature
   - Ensure adequate ventilation

4. **For 34E1K-120:** Use position A-F (5.7A RMS) for best performance
   - This is the recommended setting for this motor
   - Provides maximum torque while staying safe

## Setting the S1 Switch

### Physical Setting

1. **Power OFF** the system
2. **Locate S1 rotary switch** on CL86T driver
3. **Rotate switch** to desired position (0-F)
4. **Verify position** visually
5. **Power ON** and test

### Visual Reference

```
S1 Rotary Switch Positions:

    0  1  2  3  4  5  6  7
    ─────────────────────
    │  │  │  │  │  │  │  │
    └──┴──┴──┴──┴──┴──┴──┘
    8  9  A  B  C  D  E  F
```

**Note:** Switch typically has a pointer or indicator showing current position.

## Verification

After setting S1:

1. **Test motor operation:**
   - Send `n` command (nudge - 1 step) or `t` command (trigger)
   - Observe motor smoothness and torque

2. **Monitor temperature:**
   - Run motor for several minutes
   - Touch motor case (carefully - may be warm)
   - Should be warm but not hot
   - If too hot, reduce current setting

3. **Check for issues:**
   - Motor should run smoothly
   - No excessive vibration
   - No unusual sounds
   - Adequate torque for your application

## Troubleshooting

### Motor Lacks Torque
- **Possible cause:** Current too low
- **Solution:** Increase S1 setting to next range (if motor rating allows)
- **Example:** Move from position 4 (5.0A) to position A (5.7A)

### Motor Overheats
- **Possible cause:** Current too high for motor rating
- **Solution:** Decrease S1 setting to lower range
- **Example:** Move from position A (5.7A) to position 4 (5.0A)

### Motor Runs But Feels Weak
- **Possible cause:** Current setting below motor's optimal rating
- **Solution:** Check motor datasheet, increase if within rating

### Power Supply Shuts Down
- **Possible cause:** Current draw exceeds 5.8A limit
- **Solution:** Reduce S1 setting (use position 4 or lower)

## Current vs. Performance

### Lower Current (Position 0-3: 4.0A RMS)
- **Pros:**
  - Lower power consumption
  - Less heat generation
  - Safer for motor
- **Cons:**
  - Less torque
  - May not reach full motor potential

### Medium Current (Position 4-9: 5.0A RMS) ⭐ Recommended Start
- **Pros:**
  - Good balance of torque and safety
  - Works for most NEMA 34 motors
  - Within power supply limits
- **Cons:**
  - May be insufficient for high-torque applications

### Higher Current (Position A-F: 5.7A RMS)
- **Pros:**
  - Maximum torque
  - Best performance
- **Cons:**
  - Higher power consumption
  - More heat generation
  - Close to power supply limit
  - Only use if motor is rated for this

## Quick Reference

```
Recommended Starting Point:
S1 Position: 4 (or any 4-9)
RMS Current: 5.0A
Peak Current: 7.0A

Adjust based on:
- Motor datasheet rating
- Actual performance needs
- Temperature monitoring
```

## Safety Reminders

⚠️ **Always power OFF before changing S1 switch**
⚠️ **Never exceed motor's rated current**
⚠️ **Monitor motor temperature during initial testing**
⚠️ **Start with conservative setting and increase gradually**
⚠️ **Refer to motor datasheet for exact specifications**

## Additional Resources

- **34E1K-120 Datasheet:** Check for exact current rating
- **CL86T Manual:** See `CL86T (V4.0) manual.pdf` for complete specifications
- **Power Supply:** LE-350-60 provides 5.8A max - stay within this limit

## Next Steps

1. **Check motor datasheet** for rated current
2. **Set S1 switch** to appropriate position
3. **Test motor operation** with serial commands
4. **Monitor temperature** during operation
5. **Adjust if needed** based on performance and temperature

