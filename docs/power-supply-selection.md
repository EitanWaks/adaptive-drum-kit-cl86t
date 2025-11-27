# Power Supply Selection Guide

## Overview

Proper power supply selection is critical for optimal CL86T driver and motor performance. This guide covers voltage selection, current requirements, and power supply sharing.

## Power Supply Requirements

### Voltage Range

| Parameter | Minimum | Maximum | Unit | Our Setting |
|-----------|---------|---------|------|-------------|
| **DC Input** | 24 | 110 | VDC | 60V DC ✅ |
| **AC Input** | 18 | 80 | VAC | Not used |

**Our Configuration:**
- Using LE-350-60 power supply: **60V DC**
- Within driver's 24-110VDC range ✅
- Provides good balance of speed and reliability
- Power supply adjustable range: 54V-66V (±10%)

### Current Requirements

| Component | Current Draw | Notes |
|-----------|--------------|-------|
| **CL86T Driver** | Up to 5.7A RMS | Set by S1 switch (position A) |
| **34E1K-120 Motor** | 5.7A RMS | Per phase, at maximum setting |
| **Power Supply** | 5.8A max | LE-350-60 capacity |
| **Safety Margin** | 0.1A (1.7%) | ⚠️ Very close to limit - monitor carefully |

**Our Configuration:**
- Motor rated: 6.0A
- Driver maximum: 5.7A RMS (position A)
- Power supply: 5.8A max
- **Status:** Within limits, but close to maximum

## Voltage Selection Guidelines

### General Principles

1. **Supply voltage determines high-speed performance:**
   - Higher voltage = higher possible motor speed
   - Higher voltage = more torque at high speeds (>300 RPM)
   - Higher voltage = more noise and heating

2. **Output current determines torque:**
   - Particularly important at lower speeds
   - Set via S1 rotary switch
   - Must match motor rating

3. **Speed vs. Voltage Trade-off:**
   - **Low speed requirements:** Use lower voltage (less noise, less heat, better reliability)
   - **High speed requirements:** Use higher voltage (more torque at speed)

### Our Application (Adaptive Drum Kit)

**Recommended Voltage: 50-70V DC**

**Why 60V DC is Good:**
- ✅ Provides good torque at moderate speeds
- ✅ Not too high (reduces noise and heating)
- ✅ Within driver's optimal range
- ✅ Leaves headroom for voltage fluctuations
- ✅ Good balance for drum mechanism control

**Considerations:**
- Our application doesn't require very high speeds
- Precision and reliability are more important than maximum speed
- Lower voltage reduces noise (important for musical applications)
- Lower voltage reduces heating (better for continuous operation)

## Voltage Selection Factors

### 1. Power Line Voltage Fluctuation

**Important:** Power supply voltage can fluctuate:
- AC line voltage varies (typically ±10%)
- DC power supplies may have ripple
- **Solution:** Leave headroom above minimum voltage

**Example:**
- If minimum needed is 50V, use 60V supply
- Provides 10V headroom for fluctuations

### 2. Back-EMF Voltage

**During motor deceleration:**
- Motor generates back-EMF voltage
- This voltage charges back into power supply
- Can cause over-voltage protection if too high

**Solution:**
- Use power supply with voltage regulation
- Leave headroom for back-EMF
- Our 60V supply with 110V driver limit provides good margin

### 3. Motor Speed Requirements

| Application Speed | Recommended Voltage | Notes |
|------------------|---------------------|-------|
| **Low speed (<300 RPM)** | 24-50V DC | Lower noise, less heat |
| **Medium speed (300-1000 RPM)** | 50-70V DC | ⭐ Our application |
| **High speed (>1000 RPM)** | 70-110V DC | Maximum torque at speed |

**Our Application:**
- Moderate speed requirements
- 50-70V DC is optimal
- 60V DC is perfect choice ✅

### 4. Noise and Heating

**Higher Voltage Effects:**
- More motor vibration at lower speeds
- More heating in motor and driver
- More electrical noise
- May trigger over-voltage protection

**Lower Voltage Benefits:**
- Less noise (important for musical applications)
- Less heating (better reliability)
- Smoother operation at low speeds
- Better for continuous operation

## Power Supply Sharing

### Multiple Drivers

If using multiple CL86T drivers:

**Power Supply Requirements:**
- Total current capacity = sum of all drivers
- Example: 3 drivers × 5.7A = 17.1A minimum
- Add 20% margin: 17.1A × 1.2 = 20.5A recommended

**Connection Method:**
- ✅ **DO:** Connect each driver directly to power supply separately
- ❌ **DON'T:** Daisy-chain power connections between drivers
- **Why:** Prevents cross-interference between drivers
- **Method:** Use separate wires from power supply to each driver

**Connection Diagram:**
```
Power Supply
    │
    ├─── Driver 1 (VCC/GND)
    ├─── Driver 2 (VCC/GND)
    └─── Driver 3 (VCC/GND)
```

**NOT:**
```
Power Supply
    │
    └─── Driver 1 (VCC/GND)
         │
         └─── Driver 2 (VCC/GND)  ❌ Don't do this
              │
              └─── Driver 3 (VCC/GND)
```

## Power Supply Selection for 34E1K-120

### Recommended Power Supplies

| Power Supply | Voltage | Current | Suitability | Notes |
|--------------|---------|---------|-------------|-------|
| **LE-350-60** | 60V DC | 5.8A | ⭐ **Recommended** | Our current setup |
| 50V, 6A | 50V DC | 6A | Good | Lower voltage, less noise |
| 70V, 6A | 70V DC | 6A | Good | Higher speed capability |
| 24V, 10A | 24V DC | 10A | Acceptable | Lower speed, very quiet |

### LE-350-60 Power Supply

**Physical Specifications:**
- **Model:** LE-350-60 S Single Output
- **Dimensions:** 215 × 115 × 30mm (8.5" × 4.5" × 1.2")
- **Weight:** Not specified (check manufacturer)
- **Installation:** Vertical or horizontal mounting
- **Cooling:** Fan cooling

**Input Specifications:**
- **Input Voltage:** 115/230V AC ±20% (auto-select or manual)
- **Input Frequency:** 47-63 Hz
- **Inrush Current:** 40A @ 230VAC (cold state)
- **EMI Filtering:** Internal EMI filter included

**Output Specifications:**
- **Output Voltage:** 60V DC
- **Output Current:** 5.8A maximum
- **Rated Power:** 350W
- **Voltage Stability:** 0.5%
- **Linear Adjustment Rate:** ≤0.5%
- **Ripple and Noise:** ≤1% Vout (≤0.6V)
- **Temperature Coefficient:** ≤0.02%/°C
- **Voltage Adjustable Range:** ±10% (54V-66V)
- **Efficiency:** ≥89.5%
- **Rise Time:** 50ms @ 230VAC, full load
- **Holding Time:** 20ms @ 230VAC, full load

**Protection Features:**
- **Short Circuit Protection:** Self-recovery
- **Overload Protection:** 5 seconds @ 180-220% (self-recovery)
- **Mean Time Between Failures:** MTBF ≥ 5×10⁵ hours

**Safety Specifications:**
- **Dielectric Strength:**
  - Input to Output: 1500V AC / 1 minute
  - Input to Frame Ground: 1500V AC / 1 minute
  - Output to Frame Ground: 500V AC / 1 minute
- **Insulation Resistance:** ≥100MΩ
- **Leakage Current:** <0.5mA @ 230VAC
- **Safety Standards:** GB4943, EN60950, UL60950 compliant
- **EMC Standards:** EN55022 Class C, EN61000-3-2/3, EN61000-4-2/3/4/5/6/8/11

**Environmental Specifications:**
- **Operating Temperature:**
  - 0-45°C @ 100% load
  - -10°C @ 80% load
  - 60°C @ 40% load
- **Storage Temperature:** -20°C to 85°C
- **Relative Humidity:** 20-90% RH (no condensation)

**Warranty:** 1 year warranty

**Suitability for Our Application:**
- ✅ **Voltage:** 60V DC (optimal range for CL86T)
- ✅ **Current:** 5.8A (sufficient for 5.7A motor current)
- ⚠️ **Note:** Close to maximum - monitor current draw
- ✅ **Efficiency:** ≥89.5% (excellent)
- ✅ **Protection:** Short circuit and overload protection
- ✅ **EMI Filter:** Internal filter included (good for noise immunity)
- ✅ **Reliability:** High MTBF (500,000 hours)
- ✅ **Cooling:** Fan cooling for continuous operation

**Considerations:**
- Current capacity is close to maximum (5.7A motor / 5.8A supply)
- If adding more drivers, will need larger power supply
- For single motor, works well
- Fan cooling ensures good thermal management
- Internal EMI filter helps with noise immunity
- Self-recovery protection prevents permanent shutdowns

## Voltage vs. Performance

### Speed Performance

**Higher Voltage Benefits:**
- More torque at high speeds (>300 RPM)
- Better step retention at high speeds
- Can achieve higher maximum speeds

**Our Application:**
- Moderate speeds (100-2000 steps/sec)
- 60V provides excellent performance
- No need for higher voltage

### Torque Performance

**Torque is primarily determined by:**
- Current setting (S1 switch)
- Motor specifications
- Voltage has less effect at low speeds

**Our Application:**
- 5.7A RMS current provides excellent torque
- 60V is sufficient for our speed range
- No need for higher voltage for torque

## Safety Considerations

### Over-Voltage Protection

**CL86T Protection:**
- Driver has over-voltage protection
- Will shut down if voltage exceeds limits
- Protects driver and motor

**Prevention:**
- Use power supply within 24-110VDC range
- Account for voltage fluctuations
- Account for back-EMF during deceleration
- Our 60V supply is well within limits ✅

### Current Protection

**Power Supply:**
- Ensure power supply can handle maximum current
- Add 20% margin for safety
- Our 5.8A supply for 5.7A motor is close - monitor carefully

**Driver:**
- Current limited by S1 switch setting
- Over-current protection built-in
- Will reduce current or shut down if exceeded

## Troubleshooting Power Supply Issues

### Power Supply Shuts Down

**Possible Causes:**
- Current draw exceeds supply capacity (5.8A)
- Overload protection triggered (180-220% for 5 seconds)
- Short circuit protection activated
- Power supply fault
- Input voltage out of range

**Solutions:**
- Check current draw (should be <5.8A)
- Reduce S1 current setting if needed (try position 4: 5.0A)
- Verify power supply voltage is within range (54V-66V)
- Check for short circuits
- Verify input voltage (115/230V AC ±20%)
- Wait for self-recovery (protection is self-recovering)
- Check power supply fan is working (fan cooling)

### Power Supply Overload Protection

**LE-350-60 Protection:**
- **Overload:** 5 seconds @ 180-220% (10.4-12.8A)
- **Self-Recovery:** Automatic after condition clears
- **Short Circuit:** Self-recovery

**If Protection Triggers:**
1. Reduce motor current (lower S1 setting)
2. Check for mechanical binding
3. Verify no short circuits
4. Wait for automatic recovery
5. Monitor current draw

### Power Supply Voltage Issues

**Voltage Specifications:**
- **Output:** 60V DC ±0.5% (59.7-60.3V)
- **Adjustable:** ±10% (54V-66V)
- **Ripple:** ≤1% (≤0.6V)

**If Voltage is Incorrect:**
- Check output voltage with multimeter
- Verify no excessive load
- Check input voltage (115/230V AC)
- Adjust if within ±10% range
- Check for loose connections

### Over-Voltage Protection Triggered

**Possible Causes:**
- Power supply voltage too high
- Back-EMF during deceleration
- Voltage fluctuations

**Solutions:**
- Reduce power supply voltage
- Add voltage regulation
- Check power supply specifications
- Ensure adequate headroom

### Insufficient Torque

**Possible Causes:**
- Voltage too low for speed
- Current setting too low
- Power supply voltage sagging

**Solutions:**
- Increase voltage (if speed is high)
- Increase S1 current setting (if within motor rating)
- Check power supply voltage under load
- Ensure adequate power supply capacity

## Quick Reference

**For 34E1K-120 Motor:**
- **Recommended Voltage:** 50-70V DC (we use 60V)
- **Current Setting:** Position A (5.7A RMS)
- **Power Supply:** LE-350-60 (60V, 5.8A, 350W)
- **Current Margin:** 0.1A (1.7%) - ⚠️ Monitor carefully
- **Connection:** Direct to power supply (no daisy-chaining)
- **Cooling:** Fan cooling (ensure adequate ventilation)
- **Protection:** Short circuit and overload (self-recovery)

**For Multiple Motors:**
- **Voltage:** Same (50-70V DC)
- **Current:** Sum of all motors + 20% margin
- **Connection:** Each driver separately to power supply
- **Note:** LE-350-60 insufficient for multiple motors - need larger supply

## LE-350-60 Power Supply Specifications Summary

| Parameter | Specification | Notes |
|-----------|--------------|-------|
| **Model** | LE-350-60 S | Single output |
| **Dimensions** | 215×115×30mm | 8.5"×4.5"×1.2" |
| **Input** | 115/230V AC ±20% | Auto-select or manual |
| **Output** | 60V DC, 5.8A | 350W rated |
| **Efficiency** | ≥89.5% | Excellent |
| **Ripple** | ≤1% (≤0.6V) | Low noise |
| **Protection** | Short circuit, Overload | Self-recovery |
| **Cooling** | Fan cooling | Continuous operation |
| **EMI Filter** | Internal | Noise immunity |
| **MTBF** | ≥500,000 hours | High reliability |
| **Warranty** | 1 year | Manufacturer warranty |

## Additional Resources

- **CL86T Manual:** See `CL86T (V4.0) manual.pdf` for complete specifications
- **Power Supply Manual:** See LE-350-60 datasheet
- **Motor Datasheet:** See 34E1K-120 specifications

