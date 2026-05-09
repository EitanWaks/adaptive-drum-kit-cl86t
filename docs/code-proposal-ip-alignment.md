# Code Proposal: Alignment with IP Document Requirements

## Executive Summary

The current CL86T firmware provides basic serial command motor control. The IP document describes a comprehensive **Adaptive Motorized Percussion Pedal System** requiring:

- **Multiple input modalities** (mouthguard, EMG, sip-and-puff, head motion, hand controls)
- **Expressive motion profiles** (heel-down, heel-up, heel-toe, doubles, ghost notes, accented strikes)
- **Low-latency control** (≥1kHz update rates, sub-millisecond sensor acquisition)
- **Closed-loop feedback** (encoder position tracking, torque sensing)
- **Calibration routines** (travel limits, input normalization)
- **Multiplexing** (multiple actuators for kick + hi-hat)
- **Hi-hat specific profiles** (chick, splash, bark, tremolo, continuous openness)

## Proposed Architecture

### 1. Modular Input Device Layer

**New Files:**
- `include/input_devices.h` - Input device interface definitions
- `src/input_devices/mouthguard.cpp` - Mouthguard/bite sensor interface
- `src/input_devices/emg.cpp` - EMG sensor interface
- `src/input_devices/sip_puff.cpp` - Sip-and-puff interface
- `src/input_devices/head_motion.cpp` - IMU/head motion interface
- `src/input_devices/hand_control.cpp` - Hand/stick-mounted controls

**Key Features:**
- Abstract input device interface supporting both digital (on/off) and analog (continuous) inputs
- Input fusion for multiple simultaneous inputs
- Input normalization and calibration
- Low-latency sensor acquisition (<1ms)

### 2. Motion Profile System

**New Files:**
- `include/motion_profiles.h` - Motion profile definitions
- `src/motion_profiles/kick_profiles.cpp` - Bass drum motion profiles
- `src/motion_profiles/hihat_profiles.cpp` - Hi-hat motion profiles
- `src/motion_profiles/profile_engine.cpp` - Profile execution engine

**Kick Drum Profiles:**
- Feathering (quiet quarter-notes)
- Heel-down strokes
- Heel-up strokes
- Heel-toe (double hits from single gesture)
- Slide technique
- Swivel technique
- Quick doubles/triples
- Staccato "click" emphasis
- Long-throw power strokes
- Ghost notes (low-amplitude, low-velocity)

**Hi-Hat Profiles:**
- Foot-chick
- Foot-splash (timed open-close)
- Bark (aggressive partial-open accent)
- Controlled continuous openness (0-100%)
- Rapid "sizzle" tremulations
- Alternating open/closed articulations

**Profile Features:**
- User-editable response curves (linear, logarithmic, exponential, S-curves, splines)
- Velocity layers
- Humanization (micro-timing and micro-velocity variation)
- Swing parameters
- Genre-aware presets (jazz, rock, funk, metal, Latin, EDM)

### 3. Controller and Mapping System

**New Files:**
- `include/controller.h` - Main controller interface
- `src/controller/mapping_engine.cpp` - Input-to-motion mapping
- `src/controller/response_curves.cpp` - Response curve implementations
- `src/controller/presets.cpp` - Genre presets and user profiles

**Key Features:**
- Continuous pedal motion profiles (not binary on/off)
- Timing, position trajectory, velocity, and force/torque characteristics
- Parametric and adaptive profiles
- Learning mode (capture exemplar motions from able-bodied drummer)
- Machine learning adaptation to user's evolving abilities

### 4. Closed-Loop Feedback System

**New Files:**
- `include/feedback.h` - Feedback sensor interface
- `src/feedback/encoder_feedback.cpp` - Encoder position tracking
- `src/feedback/torque_estimation.cpp` - Current-based torque estimation
- `src/feedback/contact_sensors.cpp` - Beater/cymbal contact detection

**Key Features:**
- Position encoder feedback (using CL86T's closed-loop capability)
- Torque estimation from motor current
- Beater-to-head contact detection (for calibration)
- Travel limit detection
- Adaptive calibration routines

### 5. High-Frequency Control Loop

**New Files:**
- `include/control_loop.h` - Control loop interface
- `src/control_loop/main_loop.cpp` - High-frequency control loop (≥1kHz)

**Key Features:**
- Control loop update rates at ≥1kHz
- Deterministic jitter control
- Non-blocking operation
- Real-time profile execution
- Low end-to-end latency (<15ms target, <10ms preferred)

### 6. Calibration System

**New Files:**
- `include/calibration.h` - Calibration interface
- `src/calibration/travel_calibration.cpp` - Travel limit calibration
- `src/calibration/input_calibration.cpp` - Input range normalization
- `src/calibration/adaptive_calibration.cpp` - Adaptive learning routines

**Key Features:**
- Automatic travel limit detection
- Input range normalization
- Beater contact point calibration
- User-guided calibration routines
- Adaptive learning over time

### 7. Multiplexing System

**New Files:**
- `include/multiplexer.h` - Multiplexing interface
- `src/multiplexer/actuator_router.cpp` - Actuator routing logic

**Key Features:**
- Route commands to multiple actuators (kick + hi-hat)
- Input zone detection
- Gesture-based routing
- Pressure/velocity threshold routing
- Mode switches and scene/preset changes
- Priority logic for conflict resolution

### 8. Safety and Fail-Safe

**New Files:**
- `include/safety.h` - Safety system interface
- `src/safety/failsafe.cpp` - Fail-safe mechanisms

**Key Features:**
- Quick-release clutch (electronic disengagement)
- Dead-man thresholds
- Travel soft-limits
- Thermal/current protection
- Power-loss safe states (release pedal or open hi-hat)

## Implementation Priority

### Phase 1: Core Infrastructure (Weeks 1-2)
1. ✅ Modular input device layer (start with analog input simulation)
2. ✅ Motion profile system (basic profiles: heel-down, heel-up, doubles)
3. ✅ High-frequency control loop (1kHz update rate)
4. ✅ Closed-loop feedback (encoder position tracking)

### Phase 2: Expressive Profiles (Weeks 3-4)
1. ✅ Complete kick drum profiles (all techniques)
2. ✅ Hi-hat profiles (chick, splash, bark, tremolo)
3. ✅ Response curves (linear, exponential, S-curve)
4. ✅ Ghost notes and accented strikes

### Phase 3: Advanced Features (Weeks 5-6)
1. ✅ Calibration system
2. ✅ Multiplexing (dual actuator support)
3. ✅ Learning mode
4. ✅ Genre presets

### Phase 4: Input Devices (Weeks 7-8)
1. ✅ Mouthguard interface
2. ✅ EMG interface
3. ✅ Sip-and-puff interface
4. ✅ Head motion interface

## Code Structure Changes

### Current Structure:
```
src/
  main.cpp          (basic serial commands)
include/
  pins.h            (pin definitions)
```

### Proposed Structure:
```
src/
  main.cpp                    (initialization, main loop)
  controller/
    mapping_engine.cpp        (input-to-motion mapping)
    response_curves.cpp       (response curve implementations)
    presets.cpp              (genre presets)
  motion_profiles/
    kick_profiles.cpp         (bass drum profiles)
    hihat_profiles.cpp        (hi-hat profiles)
    profile_engine.cpp        (profile execution)
  input_devices/
    mouthguard.cpp            (bite sensor interface)
    emg.cpp                   (EMG sensor interface)
    sip_puff.cpp              (sip-and-puff interface)
    head_motion.cpp           (IMU interface)
    hand_control.cpp          (hand/stick controls)
  feedback/
    encoder_feedback.cpp      (position tracking)
    torque_estimation.cpp     (current-based torque)
    contact_sensors.cpp       (beater contact detection)
  control_loop/
    main_loop.cpp             (high-frequency control)
  calibration/
    travel_calibration.cpp    (travel limits)
    input_calibration.cpp     (input normalization)
  multiplexer/
    actuator_router.cpp        (multi-actuator routing)
  safety/
    failsafe.cpp              (safety mechanisms)

include/
  pins.h                      (pin definitions)
  input_devices.h             (input device interface)
  motion_profiles.h           (profile definitions)
  controller.h                (controller interface)
  feedback.h                  (feedback interface)
  control_loop.h              (control loop interface)
  calibration.h               (calibration interface)
  multiplexer.h               (multiplexing interface)
  safety.h                    (safety interface)
```

## Key Code Changes to Current Implementation

### 1. Replace Serial Command Interface

**Current:** Simple serial commands (`S`, `H`, `0`, `N`, `M`, `T`, `I`, `E`, `P`, `L`, `C`, `D`, `A`, speed `+/-`, and `?`)

**Proposed:** Input device abstraction layer that can accept:
- Analog inputs (0-1023 from ADC, or 0-5V)
- Digital inputs (on/off switches)
- Multiple simultaneous inputs (fused signals)

### 2. Replace Simple Movement Commands

**Current:** `motor.move(1000)` for fixed step movements

**Proposed:** Motion profile execution:
```cpp
// Example: Execute heel-down stroke profile
MotionProfile profile = createHeelDownProfile(inputIntensity);
profileEngine.execute(profile);
```

### 3. Add High-Frequency Control Loop

**Current:** `motor.run()` called in main loop with delay

**Proposed:** Dedicated high-frequency control loop:
```cpp
void controlLoop() {
  static unsigned long lastUpdate = 0;
  unsigned long now = micros();

  if (now - lastUpdate >= 1000) {  // 1kHz = 1000 microseconds
    // Read input devices
    InputState input = readInputDevices();

    // Map to motion profile
    MotionProfile profile = mappingEngine.map(input);

    // Execute profile
    profileEngine.update(profile);

    // Update motor
    motor.run();

    // Update feedback
    feedbackSystem.update();

    lastUpdate = now;
  }
}
```

### 4. Add Encoder Feedback

**Current:** Open-loop position tracking via `motor.currentPosition()`

**Proposed:** Closed-loop feedback using CL86T encoder:
```cpp
// Read encoder position from CL86T (if accessible via serial/analog)
// Or use CL86T's internal closed-loop correction
long actualPosition = readEncoderPosition();
long commandedPosition = motor.currentPosition();

// Detect position error (step loss)
long positionError = actualPosition - commandedPosition;

// Compensate if error exceeds threshold
if (abs(positionError) > ERROR_THRESHOLD) {
  motor.setCurrentPosition(actualPosition);
}
```

### 5. Expand Calibration Routines

**Current:** Basic FSR baseline/noise calibration and HX711 zeroing via `C`; no travel-limit or input-normalization profiles yet.

**Proposed:** Automatic and manual calibration:
```cpp
void calibrateTravelLimits() {
  // Move to find physical limits
  // Detect beater contact
  // Store limits in EEPROM
}

void calibrateInputRange(InputDevice* device) {
  // Read min/max input values
  // Normalize to 0-1.0 range
  // Store calibration in EEPROM
}
```

## Example: Heel-Down Stroke Profile

```cpp
MotionProfile createHeelDownProfile(float inputIntensity) {
  MotionProfile profile;

  // Scale travel based on input intensity
  float travelScale = mapInputToTravel(inputIntensity);

  // Heel-down: slower acceleration, controlled descent
  profile.acceleration = 500.0f;  // steps/sec²
  profile.maxSpeed = 800.0f * travelScale;  // steps/sec
  profile.targetPosition = HOME_POSITION + (MAX_TRAVEL * travelScale);

  // Smooth deceleration before contact
  profile.decelerationDistance = 50;  // steps

  return profile;
}
```

## Example: Hi-Hat Chick Profile

```cpp
MotionProfile createHiHatChickProfile() {
  MotionProfile profile;

  // Chick: quick close-open-close
  profile.sequence = {
    {CLOSED_POSITION, 1000.0f, 0.05f},  // Close quickly (50ms)
    {OPEN_POSITION, 800.0f, 0.10f},    // Open (100ms)
    {CLOSED_POSITION, 1000.0f, 0.05f}   // Close again (50ms)
  };

  return profile;
}
```

## Next Steps

1. **Review and approve** this architecture proposal
2. **Prioritize features** based on immediate needs
3. **Begin Phase 1 implementation** (core infrastructure)
4. **Test with simulated inputs** before hardware integration
5. **Iterate based on user feedback**

## Questions for Discussion

1. Which input devices should be prioritized? (mouthguard, EMG, sip-and-puff, etc.)
2. Should we start with single actuator (kick) or dual actuator (kick + hi-hat)?
3. What latency requirements are acceptable? (IP mentions <15ms, <10ms preferred)
4. Should calibration be automatic or user-guided?
5. Do we need genre presets immediately, or can we start with basic profiles?










