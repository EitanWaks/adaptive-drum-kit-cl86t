# CL86T DIP Switch Settings

## Recommended Settings for Initial Setup

Based on the firmware configuration and motor specifications, here are the recommended DIP switch settings:

### Pulse/Revolution Settings (SW1-SW4)

| Switch | Position | Setting |
| ------ | -------- | ------- |
| SW1    | **ON**   |         |
| SW2    | **ON**   |         |
| SW3    | **ON**   |         |
| SW4    | **ON**   |         |

**Result: 200 pulse/revolution** (Full step mode)

This matches the 34E1K-120 motor specification of 200 steps per revolution (1.8° per step).

### Mode Settings (SW5-SW8)

| Switch | Function     | Position | Setting        | Reason                                                              |
| ------ | ------------ | -------- | -------------- | ------------------------------------------------------------------- |
| SW5    | Direction    | **ON**   | Clockwise (CW) | ⚠️ **CORRECTED:** ON = CW, OFF = CCW                                 |
| SW6    | Control Mode | **OFF**  | Closed Loop    | Use closed-loop mode for better accuracy and step loss compensation |
| SW7    | Pulse Mode   | **OFF**  | Pul/Dir        | Matches our code which uses STEP and DIR pins                       |
| SW8    | Brake/Pend   | **OFF**  | Brake Output   | Brake mode for safer operation                                      |

## Complete DIP Switch Configuration

```
┌─────────────────────────────────────┐
│         CL86T DIP SWITCHES          │
│                                     │
│  SW1: [ON]  ← Pulse/Rev             │
│  SW2: [ON]  ← Pulse/Rev             │
│  SW3: [ON]  ← Pulse/Rev             │
│  SW4: [ON]  ← Pulse/Rev             │
│                                     │
│  SW5: [ON]  ← Direction (CW)         │
│  SW6: [OFF] ← Control (Closed)      │
│  SW7: [OFF] ← Pulse (Pul/Dir)       │
│  SW8: [OFF] ← Mode (BRK)            │
│                                     │
└─────────────────────────────────────┘
```

**Summary: All switches ON for SW1-SW4, SW5 ON (CW), SW6-SW8 OFF (Closed Loop, Pul/Dir, Brake)**

## Explanation of Each Setting

### SW1-SW4: 200 Pulse/Revolution (All ON)

- **Why:** The 34E1K-120 motor has 200 steps per revolution in full-step mode
- **Alternative:** If you want microstepping (e.g., 400, 800, 1600, 3200 steps/rev), adjust these switches according to CL86T manual
- **Note:** If you change this, you'll need to update `STEPS_PER_REV` in `include/pins.h`

### SW5: Direction (ON = Clockwise, OFF = Counterclockwise)

- **⚠️ CORRECTED:** ON = Clockwise (CW), OFF = Counterclockwise (CCW)
- **Why:** Standard clockwise direction
- **Our Setting:** ON (Clockwise)
- **Alternative:** If motor moves opposite to expected, you can either:
  - Set SW5 to OFF (counterclockwise), OR
  - Change `DIR_INVERT = true` in `include/pins.h`
- **Recommendation:** Leave ON and use software inversion if needed

### SW6: Control Mode (OFF = Closed Loop)

- **Why:** The CL86T is a closed-loop driver - use this feature for:
  - Automatic step loss compensation
  - Better position accuracy
  - Stall detection
- **Alternative:** Set to ON for open-loop mode (simpler but less accurate)
- **Note:** For closed-loop mode, you may need to connect encoder (if motor has one)

### SW7: Pulse Mode (OFF = Pul/Dir)

- **Why:** Our code uses standard step/direction control:
  - Pin 2 = STEP (PUL)
  - Pin 3 = DIR
- **Alternative:** Set to ON for CW/CCW mode (would require code changes)
- **Must match:** This must match the code implementation

### SW8: Brake/Pend (OFF = Brake Output, ON = Pend Output)

- **Why:** Brake output provides safer operation
- **Our Setting:** OFF (Brake Output)
- **Alternative:** Set to ON for Pend output (if you have specific requirements)
- **Recommendation:** Start with Brake (OFF) for initial testing

## Alternative Configurations

### For Open-Loop Operation (Simpler Testing)

If you want to start with open-loop mode for simpler initial testing:

```
SW1-SW4: All ON (200 pulse/rev)
SW5: ON (Clockwise)
SW6: ON (Open Loop) ← Changed
SW7: OFF (Pul/Dir)
SW8: OFF (Brake)
```

**Note:** You'll lose closed-loop benefits but may be easier for initial setup.

### For Microstepping (Higher Resolution)

If you want higher resolution (e.g., 1600 steps/rev):

```
SW1: ON
SW2: ON
SW3: ON
SW4: OFF ← Changed (check CL86T manual for exact combination)
SW5-SW8: Same as recommended above
```

**Important:** If you change microstepping, update `STEPS_PER_REV` in `include/pins.h` to match.

## Verification

After setting the DIP switches:

1. **Power off** the system
2. **Set all switches** according to recommendations
3. **Double-check** switch positions
4. **Power on** and test with serial commands:
   - Send `n` command (nudge - 1 step) to test basic movement
   - Send `m` then `90` to map 90 degrees, then `t` to trigger
   - Verify motor rotates and returns accurately
   - Check direction matches expectations

## Troubleshooting

### Motor Doesn't Move
- Verify SW1-SW4 are all ON (200 pulse/rev)
- Check SW7 is OFF (Pul/Dir mode)
- Verify SW6 is OFF (closed loop) or ON (open loop) - try both

### Wrong Direction
- Toggle SW5 (ON = CW, OFF = CCW)
- Or change `DIR_INVERT` in `pins.h`

### Motor Moves But Position is Wrong
- Check SW1-SW4 settings match `STEPS_PER_REV` in code
- Verify microstepping settings if using microstepping

### Closed-Loop Not Working
- Ensure SW6 is OFF (closed loop mode)
- Check if encoder is connected (if required for closed-loop)
- Try open-loop mode (SW6 = ON) for testing

## Current Setting (S1 Rotary Switch)

**Important:** In addition to DIP switches, you must also configure the S1 rotary switch for current setting.

**Quick Recommendation:** Start with **Position 4** (5.0A RMS) for most NEMA 34 motors.

**For complete S1 switch settings, see [Current Settings Guide](current-settings.md)**

## CL86T Driver Specifications

### Key Features

- **Input Voltage:** 18-80VAC or 24-110VDC (our 60V DC is within range)
- **Max Pulse Frequency:** 500 KHz (our speeds are well below this)
- **Resolution Range:** 200-51,200 steps/revolution (via SW1-SW4)
- **Optically Isolated Inputs:** 5V or 24V (we use 5V from Arduino)
- **Protection Features:** Over-voltage, over-current, position error detection

**For complete CL86T specifications, see [CL86T Specifications Guide](cl86t-specifications.md)**

## Reference

- **CL86T Manual:** See `CL86T (V4.0) manual.pdf` for complete DIP switch reference
- **CL86T Specifications:** See [CL86T Specifications Guide](cl86t-specifications.md)
- **Current Settings:** See [Current Settings Guide](current-settings.md) for S1 rotary switch
- **Motor Specs:** 34E1K-120 has 200 steps/rev in full-step mode
- **Code Config:** See `include/pins.h` for software configuration

## Quick Reference Card

```
Initial Setup:
SW1: ON   SW2: ON   SW3: ON   SW4: ON   (200 pulse/rev, full step)
SW5: ON   SW6: OFF  SW7: OFF  SW8: OFF  (CW, Closed Loop, Pul/Dir, Brake)
```

