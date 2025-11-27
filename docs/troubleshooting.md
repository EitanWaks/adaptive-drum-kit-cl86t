# Troubleshooting Guide

## Quick Reference

| Symptom | Likely Cause | Solution |
|---------|--------------|----------|
| Motor doesn't move | No power, wrong wiring, enable off | Check power, verify connections, check enable |
| Motor vibrates | Wrong phase wiring | Swap motor phase wires |
| Wrong direction | Direction pin inverted | Change DIR_INVERT or swap wires |
| Erratic movement | Loose connections, interference | Tighten connections, check cables |
| Arduino not responding | USB/COM port issue | Check cable, COM port, baud rate |

## Common Issues and Solutions

### Issue 1: Motor Doesn't Move at All

**Symptoms:**
- No movement when commands are sent
- No sound from motor
- Motor feels "locked" or free

**Diagnosis Steps:**

1. **Check Power Supply**
   ```
   - Is power supply plugged in and switched on?
   - Is LED/indicator on power supply lit?
   - Measure voltage at CL86T VCC terminal (should be ~60V)
   - Check power supply output with multimeter
   ```

2. **Check Motor Enable**
   ```
   - Verify PIN_EN is configured correctly
   - Check if enable signal is active (HIGH or LOW depending on EN_ACTIVE_LOW)
   - Try toggling enable with 'E' command
   ```

3. **Check Motor Connections**
   ```
   - Verify all 4 motor wires are connected
   - Check for loose connections
   - Measure motor phase resistance (should be 0.5-2.0 ohms)
   ```

4. **Check Signal Connections**
   ```
   - Verify Arduino Pin 2 connected to PUL+
   - Verify Arduino Pin 3 connected to DIR+
   - Verify Arduino GND connected to PUL- and DIR-
   - Check for continuity with multimeter
   ```

5. **Check Arduino Code**
   ```
   - Verify code uploaded successfully
   - Check serial monitor for responses
   - Try sending '?' command to verify communication
   ```

**Solutions:**

- **No Power:** Check power supply connections, verify AC input, check fuse
- **Enable Off:** Check PIN_EN configuration, verify enable signal
- **Wrong Wiring:** Re-check all connections against wiring diagram
- **Code Issue:** Re-upload code, check for errors in serial monitor

### Issue 2: Motor Vibrates but Doesn't Rotate

**Symptoms:**
- Motor makes humming/vibrating sound
- Motor shaft doesn't rotate
- Motor feels "locked" in place

**Cause:** Incorrect motor phase wiring

**Solution:**

Try these in order:

1. **Swap Phase A:**
   ```
   Swap A+ and A- connections
   ```

2. **Swap Phase B:**
   ```
   Swap B+ and B- connections
   ```

3. **Swap Both Phases:**
   ```
   Swap A+ with A- AND B+ with B-
   ```

4. **Swap Phases Between A and B:**
   ```
   Connect motor wire 1 to B+ instead of A+
   Connect motor wire 2 to B- instead of A-
   Connect motor wire 3 to A+ instead of B+
   Connect motor wire 4 to A- instead of B-
   ```

**Verification:**
After each swap, test with 'R' command (one revolution). Motor should rotate smoothly.

### Issue 3: Motor Moves in Wrong Direction

**Symptoms:**
- Motor rotates but opposite to expected
- Forward command moves backward
- Backward command moves forward

**Solutions:**

**Option 1: Software Fix (Recommended)**
```cpp
// In include/pins.h, change:
static const bool DIR_INVERT = true;  // Change from false to true
```

**Option 2: Hardware Fix**
```
Swap DIR+ and DIR- connections on CL86T
```

**Verification:**
After fix, test with 'F' (forward) and 'B' (backward) commands.

### Issue 4: Motor Moves Erratically or Stutters

**Symptoms:**
- Motor movement is jerky
- Motor skips steps
- Inconsistent positioning
- Motor makes unusual sounds

**Diagnosis Steps:**

1. **Check Connections**
   ```
   - Tighten all connections
   - Check for loose wires
   - Verify no frayed strands causing shorts
   ```

2. **Check Power Supply**
   ```
   - Measure voltage stability (should be steady ~60V)
   - Check for voltage drops under load
   - Verify power supply can handle current draw
   ```

3. **Check Signal Quality**
   ```
   - Verify signal wires are not too long (>1m may need shielding)
   - Check for interference from other devices
   - Try using twisted pair or shielded cable
   ```

4. **Check Speed Settings**
   ```
   - Motor may be running too fast
   - Try reducing speed with '-' command
   - Check acceleration settings
   ```

**Solutions:**

- **Loose Connections:** Tighten all terminals, check wire integrity
- **Power Issues:** Verify power supply capacity, check voltage stability
- **Interference:** Use shielded cables, keep signal wires away from power wires
- **Speed Too High:** Reduce speed, increase acceleration time
- **Driver Settings:** Check CL86T current limit and microstepping settings

### Issue 5: Motor Loses Steps

**Symptoms:**
- Motor doesn't reach target position
- Position drifts over time
- Inaccurate positioning

**Causes and Solutions:**

1. **Speed Too High**
   ```
   - Reduce speed with '-' command
   - Motor may not be able to follow high step rates
   - Try speeds below 1000 steps/sec initially
   ```

2. **Acceleration Too High**
   ```
   - Reduce ACCEL_SPS2 in pins.h
   - Motor needs time to accelerate
   - Try 500-1000 steps/sec²
   ```

3. **Load Too High**
   ```
   - Motor may be overloaded
   - Check if mechanical load is within motor capacity
   - Verify motor can move freely without load first
   ```

4. **Driver Current Too Low**
   ```
   - Check CL86T current limit settings
   - Increase current if below motor rating
   - Refer to CL86T manual for current adjustment
   ```

5. **Closed-Loop Not Active**
   ```
   - CL86T has closed-loop capability
   - Verify encoder is connected (if using closed-loop mode)
   - Check closed-loop settings in driver
   ```

### Issue 6: Arduino Not Responding

**Symptoms:**
- Serial monitor shows no output
- Commands have no effect
- Arduino LED not on

**Diagnosis Steps:**

1. **Check USB Connection**
   ```
   - Verify USB cable is connected
   - Try different USB cable
   - Try different USB port
   ```

2. **Check COM Port**
   ```
   - Verify correct COM port selected
   - Check Device Manager for Arduino
   - Try unplugging and replugging USB
   ```

3. **Check Serial Monitor Settings**
   ```
   - Baud rate must be 115200
   - Line ending: Both NL & CR or Newline
   - Auto-scroll enabled
   ```

4. **Check Arduino Power**
   ```
   - Arduino LED should be on
   - If LED is off, Arduino may not be powered
   - Check USB cable provides power
   ```

**Solutions:**

- **USB Issue:** Try different cable/port, check USB drivers
- **COM Port:** Select correct port, restart IDE/PlatformIO
- **Baud Rate:** Set to 115200 in serial monitor
- **Code Issue:** Re-upload code, check for upload errors

### Issue 7: Motor or Driver Overheats

**Symptoms:**
- Motor becomes hot to touch
- Driver becomes hot
- Performance degrades over time
- Driver shuts down or reduces current

**Causes and Solutions:**

#### Driver Overheating (CL86T)

1. **Mounting Orientation**
   ```
   - Mount driver VERTICALLY (heat sink fins vertical)
   - Vertical mounting maximizes heat dissipation
   - Natural convection works best in vertical orientation
   ```

2. **Insufficient Cooling**
   ```
   - Ensure adequate ventilation around driver
   - Add cooling fan nearby if temperature exceeds 35°C
   - Keep at least 50mm clearance around driver
   - If multiple drivers, keep 30mm (12 inches) spacing
   ```

3. **Current Too High**
   ```
   - Reduce S1 current setting if running at maximum (5.7A RMS)
   - Try position 4 (5.0A RMS) instead of position A
   - Monitor driver temperature during operation
   ```

4. **Maximum Temperature**
   ```
   - CL86T reliable working temperature: ≤40°C (109°F)
   - If approaching limit, add cooling fan
   - Reduce current or duty cycle if needed
   ```

#### Motor Overheating

1. **Current Too High**
   ```
   - Reduce CL86T current limit (S1 switch)
   - Motor should run at 70-80% of rated current for continuous operation
   - Our motor is 6.0A rated, running at 5.7A (95%) - monitor temperature
   ```

2. **Insufficient Cooling**
   ```
   - Ensure adequate ventilation around motor
   - Add cooling fan if needed
   - Keep motor in open air
   ```

3. **Continuous Operation**
   ```
   - Motor may need rest periods
   - Reduce duty cycle
   - Use enable pin to disable when not moving
   ```

4. **Mechanical Binding**
   ```
   - Check if motor shaft can rotate freely
   - Verify no mechanical binding
   - Reduce load if possible
   ```

### Issue 8: Power Supply Issues

**Symptoms:**
- Power supply LED off
- No voltage output
- System shuts down under load

**Diagnosis:**

1. **Check AC Input**
   ```
   - Verify AC power is connected
   - Check input voltage matches supply rating
   - Verify fuse/breaker is not tripped
   ```

2. **Check Output**
   ```
   - Measure voltage at output terminals
   - Should read ~60V DC
   - Check for short circuits
   ```

3. **Check Load**
   ```
   - Verify load doesn't exceed 5.8A
   - Check for short circuits in wiring
   - Measure current draw
   ```

**Solutions:**

- **No AC Power:** Check wall outlet, verify input voltage
- **Fuse Blown:** Replace fuse, check for short circuits
- **Overload:** Reduce load, check current draw
- **Faulty Supply:** Replace power supply if output is incorrect

## Advanced Troubleshooting

### Using Multimeter

**Voltage Measurements:**
```
- Power supply output: Should be ~60V DC
- CL86T VCC: Should match power supply
- Arduino 5V: Should be ~5V (if measuring)
- Signal pins: Should be 0V (LOW) or 5V (HIGH)
```

**Resistance Measurements:**
```
- Motor phase A: 0.5-2.0 ohms
- Motor phase B: 0.5-2.0 ohms
- Between phases: High resistance (open)
- Signal wires: Low resistance (continuity)
```

**Current Measurements:**
```
- Power supply output: Should be <5.8A
- Motor current: Check against motor rating
- Use clamp meter for non-invasive measurement
```

### Using Oscilloscope (if available)

**Check Step Pulses:**
```
- Connect probe to PUL+ terminal
- Should see square wave pulses
- Frequency should match step rate
- Pulse width should be >3 microseconds
```

**Check Direction Signal:**
```
- Connect probe to DIR+ terminal
- Should see HIGH or LOW depending on direction
- Should change when direction changes
```

### Serial Debugging

**Enable Debug Output:**
Add debug prints to code to track:
- Step commands sent
- Position updates
- Speed changes
- Enable state changes

**Monitor Serial Output:**
```
- Watch for error messages
- Check command responses
- Verify status updates
- Look for timing issues
```

## Prevention

### Best Practices

1. **Always disconnect power** before making connections
2. **Double-check wiring** before powering on
3. **Start with low speeds** and increase gradually
4. **Test without load** first, then add load
5. **Keep connections clean** and properly insulated
6. **Use appropriate wire gauge** for current
7. **Ensure proper ventilation** for cooling
8. **Regular maintenance** checks

### Regular Maintenance

- **Weekly:** Check connections, verify operation
- **Monthly:** Inspect wires, clean terminals
- **Quarterly:** Full system check, measure voltages
- **Annually:** Complete inspection, replace worn parts

## Getting Help

If issues persist:

1. **Review Documentation:**
   - Wiring diagram
   - Hardware setup guide
   - Component datasheets

2. **Check Component Manuals:**
   - CL86T manual for driver-specific issues
   - Motor datasheet for specifications
   - Power supply manual for electrical specs

3. **Test Components Individually:**
   - Test motor with different driver
   - Test driver with different motor
   - Test power supply with multimeter

4. **Contact Support:**
   - Component manufacturer support
   - Arduino community forums
   - Project maintainers

## Diagnostic Checklist

Use this checklist when troubleshooting:

- [ ] Power supply is on and outputting correct voltage
- [ ] All connections are secure and correct
- [ ] Motor wires are connected in correct pairs
- [ ] Arduino is powered and communicating
- [ ] Serial monitor is open at 115200 baud
- [ ] Code is uploaded successfully
- [ ] Motor enable signal is active
- [ ] Motor shaft can rotate freely
- [ ] No short circuits detected
- [ ] Driver settings match motor specifications
- [ ] Speed and acceleration are within limits
- [ ] No interference on signal wires
- [ ] Adequate cooling/ventilation
- [ ] Load is within motor capacity

