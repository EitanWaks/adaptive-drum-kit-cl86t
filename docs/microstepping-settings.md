# CL86T Microstepping Settings

## Overview

The CL86T driver supports 16 different microstepping resolutions configured via DIP switches SW1-SW4. This allows you to choose between maximum torque (full step) and higher resolution (microstepping).

## Microstepping Table

| Micro Step | Pulses/Rev (1.8° motor) | SW1 | SW2 | SW3 | SW4 | Resolution | Torque |
|------------|------------------------|-----|-----|-----|-----|------------|--------|
| **1** | 200 | ON | ON | ON | ON | 1.8° | ⭐ **Maximum** |
| **4** | 800 | OFF | ON | ON | ON | 0.45° | High |
| **5** | 1000 | ON | ON | ON | OFF | 0.36° | High |
| **8** | 1600 | ON | OFF | ON | ON | 0.225° | Medium |
| **10** | 2000 | OFF | ON | ON | OFF | 0.18° | Medium |
| **16** | 3200 | OFF | OFF | ON | ON | 0.1125° | Medium |
| **20** | 4000 | ON | OFF | ON | OFF | 0.09° | Medium |
| **25** | 5000 | OFF | OFF | ON | OFF | 0.072° | Medium |
| **32** | 6400 | ON | ON | OFF | ON | 0.05625° | Low |
| **40** | 8000 | ON | ON | OFF | OFF | 0.045° | Low |
| **50** | 10000 | OFF | ON | OFF | OFF | 0.036° | Low |
| **64** | 12800 | OFF | ON | OFF | ON | 0.028125° | Low |
| **100** | 20000 | ON | OFF | OFF | OFF | 0.018° | Low |
| **128** | 25600 | ON | OFF | OFF | ON | 0.0140625° | Very Low |
| **200** | 40000 | OFF | OFF | OFF | OFF | 0.009° | Very Low |
| **256** | 51200 | OFF | OFF | OFF | ON | 0.00703125° | Very Low |

## Recommended Settings

### For 34E1K-120 Motor (Our Application)

**Recommended: Full Step (1 microstep) - SW1-SW4: All ON**

| Setting | Pulses/Rev | Resolution | Torque | Use Case |
|---------|------------|------------|--------|----------|
| **Full Step** | 200 | 1.8° | ⭐ Maximum | Drum mechanism (our use) |
| **4 microsteps** | 800 | 0.45° | High | Higher precision needed |
| **8 microsteps** | 1600 | 0.225° | Medium | Smooth motion required |

**Why Full Step for Our Application:**
- Maximum torque (12Nm motor)
- Simpler control (200 steps/rev)
- Sufficient precision for drum mechanism
- Less computational overhead

## Microstepping Trade-offs

### Full Step (1 microstep) - 200 pulses/rev

**Advantages:**
- ⭐ Maximum torque per step
- Simplest control
- Fastest step response
- Best for high-torque applications

**Disadvantages:**
- Lower resolution (1.8° per step)
- More vibration at low speeds
- Less smooth motion

**Best For:**
- High torque requirements
- Simple positioning
- Our drum mechanism application

### Medium Microstepping (4-16 microsteps)

**Advantages:**
- Higher resolution
- Smoother motion
- Less vibration
- Better low-speed performance

**Disadvantages:**
- Reduced torque per step
- More complex control
- Higher pulse frequency needed

**Best For:**
- Precision positioning
- Smooth motion requirements
- Low-speed applications

### High Microstepping (32-256 microsteps)

**Advantages:**
- Very high resolution
- Very smooth motion
- Minimal vibration
- Excellent low-speed performance

**Disadvantages:**
- Significantly reduced torque
- Very high pulse frequency required
- May not be suitable for high-load applications

**Best For:**
- Ultra-precise positioning
- Very smooth motion
- Low-torque applications

## Changing Microstepping

### Step 1: Update DIP Switches

1. **Power OFF** the system
2. Set SW1-SW4 according to desired microstepping
3. **Power ON** and test

### Step 2: Update Code

If changing from full step (200), update `include/pins.h`:

```cpp
// For full step (current setting):
static const long STEPS_PER_REV = 200;

// For 4 microsteps:
static const long STEPS_PER_REV = 800;

// For 8 microsteps:
static const long STEPS_PER_REV = 1600;

// etc.
```

### Step 3: Adjust Speed Settings

Higher microstepping requires higher pulse frequencies:

```cpp
// For full step (200 steps/rev):
static const float SPEED_MAX_SPS = 2000.0f;  // 2000 steps/sec = 10 rev/sec

// For 4 microsteps (800 steps/rev):
static const float SPEED_MAX_SPS = 8000.0f;  // 8000 steps/sec = 10 rev/sec

// For 8 microsteps (1600 steps/rev):
static const float SPEED_MAX_SPS = 16000.0f; // 16000 steps/sec = 10 rev/sec
```

**Note:** Driver supports up to 500 KHz (500,000 steps/sec) in 5V mode, so even high microstepping is well within limits.

## Microstepping Selection Guide

### Choose Full Step (1) If:
- ✅ Maximum torque is required
- ✅ Simple control is preferred
- ✅ Resolution of 1.8° is sufficient
- ✅ High-speed operation needed
- ✅ **Our drum mechanism application**

### Choose Medium Microstepping (4-16) If:
- ✅ Higher precision needed (0.1-0.5°)
- ✅ Smoother motion required
- ✅ Low-speed operation
- ✅ Reduced vibration important

### Choose High Microstepping (32-256) If:
- ✅ Ultra-high precision needed (<0.1°)
- ✅ Very smooth motion critical
- ✅ Low torque requirements
- ✅ Minimal vibration essential

## Quick Reference

**Our Current Setting:**
```
SW1: ON   SW2: ON   SW3: ON   SW4: ON
Result: 1 microstep, 200 pulses/rev, 1.8° resolution, Maximum torque
```

**Common Alternatives:**
```
SW1: OFF  SW2: ON   SW3: ON   SW4: ON
Result: 4 microsteps, 800 pulses/rev, 0.45° resolution

SW1: ON   SW2: OFF  SW3: ON   SW4: ON
Result: 8 microsteps, 1600 pulses/rev, 0.225° resolution
```

## Additional Resources

- **DIP Switch Settings:** See [DIP Switch Settings Guide](dip-switch-settings.md)
- **Current Settings:** See [Current Settings Guide](current-settings.md)
- **CL86T Manual:** See `CL86T (V4.0) manual.pdf` for complete specifications

