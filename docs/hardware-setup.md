# Hardware Setup and Wiring Guide

## Hardware Stack

| Component | Model | Notes |
| --- | --- | --- |
| Microcontroller | Arduino UNO R3 | 5V logic, serial monitor at 115200 baud |
| Driver | CL86T closed-loop stepper driver | Use 5V input mode for Arduino signals |
| Motor | 34E1K-120 NEMA 34 | 1.8 degrees/step, 200 full steps/rev, 6.0A rated |
| Power supply | LE-350-60 | 60V DC, 5.8A max |
| Force sensor | DF9-16 FSR | Current firmware reads A0 only |
| Load sensors | HX711 modules | Up to two load cells on D5-D8 |

## Safety

- Disconnect AC power before wiring or changing any connection.
- The LE-350-60 outputs 60V DC. Treat it as hazardous.
- Do not short `+V` to `-V` on the power supply.
- Use 14-16 AWG wire for power wiring and 22-24 AWG wire for control/sensor wiring.
- Keep motor power wiring separated from signal and sensor wiring.
- Confirm all terminal screws are tight before applying power.
- Keep the motor shaft free to rotate during first tests.

## Correct Baseline Wiring

### Arduino to CL86T Control Signals

Use this for initial bring-up:

```text
Arduino Pin 2  ->  CL86T PUL+    Step pulse
Arduino Pin 3  ->  CL86T DIR+    Direction
Arduino GND    ->  CL86T PUL-    Signal ground
Arduino GND    ->  CL86T DIR-    Signal ground
Arduino GND    ->  CL86T ENA-    Enable common/reference
```

Do not connect Arduino Pin 4 to `ENA+` during initial bring-up. In this hardware, connecting `ENA+` to Arduino Pin 4 caused zero motor movement.

`ENA-` is different: keep `ENA-` connected to Arduino GND/common signal ground with the other negative signal terminals. Only add driven `ENA+` control after basic PUL/DIR motion works and the CL86T enable polarity/timing has been verified against the driver manual and `EN_ACTIVE_LOW` in `include/pins.h`.

Optional protection after wiring is confirmed:

```text
Arduino Pin 2  ->  1k resistor  ->  CL86T PUL+
Arduino Pin 3  ->  1k resistor  ->  CL86T DIR+
```

Do not add `ENA+` resistor wiring during initial bring-up.

### LE-350-60 Power Supply to CL86T

```text
LE-350-60 +V (any one)  ->  CL86T AC terminal
LE-350-60 -V (any one)  ->  CL86T AC terminal
LE-350-60 GND           ->  frame/safety ground as appropriate
```

The CL86T `AC` input terminals accept DC input and are non-polarized for DC, but keep the supply wiring cleanly labeled and never short `+V` to `-V`.

Typical AC input wire colors for the power supply:

```text
Blue    ->  L
Brown   ->  N
Yellow  ->  GND
```

Verify local mains wiring standards before connecting AC.

### Motor to CL86T

This is the verified 34E1K-120 motor wiring:

```text
Black  ->  CL86T A+
Green  ->  CL86T A-
Red    ->  CL86T B+
Blue   ->  CL86T B-
```

If the motor vibrates but does not rotate, swap one phase pair: either `A+`/`A-` or `B+`/`B-`.

### Encoder to CL86T

The CL86T powers the encoder from its encoder `VCC` terminal. Do not connect external 5V to encoder `VCC`.

```text
Encoder Red            ->  CL86T VCC   (5V output from driver)
Encoder Black (thin)   ->  CL86T EGND
Encoder Green          ->  CL86T EA+
Encoder White          ->  CL86T EA-
Encoder Yellow         ->  CL86T EB+
Encoder Blue           ->  CL86T EB-
Encoder Black (thick)  ->  CL86T EGND or shield ground
```

Keep encoder wiring away from motor phase wiring.

## CL86T Driver Setup

### 5V/24V Input Selector

Set the CL86T input selector to `5V` for Arduino UNO control.

### DIP Switches

Recommended initial settings:

| Switch | Position | Meaning |
| --- | --- | --- |
| SW1 | ON | 200 pulses/rev, full step |
| SW2 | ON | 200 pulses/rev, full step |
| SW3 | ON | 200 pulses/rev, full step |
| SW4 | ON | 200 pulses/rev, full step |
| SW5 | ON | Clockwise direction |
| SW6 | OFF | Closed-loop mode |
| SW7 | OFF | Step/direction pulse mode |
| SW8 | OFF | Brake output mode |

This matches `STEPS_PER_REV = 200` in `include/pins.h`. If you change SW1-SW4 for microstepping, update `STEPS_PER_REV` and speed limits in firmware.

Common alternatives:

| Pulses/rev | Microstep | SW1 | SW2 | SW3 | SW4 |
| --- | --- | --- | --- | --- | --- |
| 200 | 1 | ON | ON | ON | ON |
| 800 | 4 | OFF | ON | ON | ON |
| 1600 | 8 | ON | OFF | ON | ON |
| 3200 | 16 | OFF | OFF | ON | ON |

### S1 Current Setting

Start conservatively:

| S1 position | RMS current | Peak current | Use |
| --- | --- | --- | --- |
| 0-3 | 4.0A | 5.6A | Lower-current testing |
| 4-9 | 5.0A | 7.0A | Recommended initial bring-up |
| A-F | 5.7A | 8.0A | Higher torque after validation |

The 34E1K-120 motor is rated around 6.0A, while the CL86T maximum is 5.7A RMS. Position `A` is close to the LE-350-60 supply limit, so use it only after the system moves reliably at position `4` and temperatures are acceptable.

## Sensors

### DF9-16 FSR on A0

The current firmware is FSR-oriented and reads A0 only in debug mode.

Wire the FSR as a voltage divider:

```text
Arduino 5V  ->  FSR  ->  Arduino A0  ->  10k resistor  ->  Arduino GND
```

Expected readings:

- Not pressed: A0 near 0V, raw reading roughly 0-100.
- Pressed: A0 rises, raw reading increases significantly.

Do not use the older 1 Mohm piezo pull-down circuit for the DF9-16 FSR.

Useful commands:

```text
C  Calibrate FSR baseline and load sensor zero
P  Read FSR value on A0
D  Continuous A0 diagnostic stream
A  Toggle FSR auto-trigger mode
```

### HX711 Load Sensors

HX711 module 1:

```text
HX711 VCC  ->  Arduino 5V
HX711 GND  ->  Arduino GND
HX711 DT   ->  Arduino Pin 5
HX711 SCK  ->  Arduino Pin 6
```

HX711 module 2:

```text
HX711 VCC  ->  Arduino 5V
HX711 GND  ->  Arduino GND
HX711 DT   ->  Arduino Pin 7
HX711 SCK  ->  Arduino Pin 8
```

Typical 4-wire load cell to HX711:

```text
Load cell E+ (Red)    ->  HX711 E+
Load cell E- (Black)  ->  HX711 E-
Load cell A+ (White)  ->  HX711 A+
Load cell A- (Green)  ->  HX711 A-
```

Add a 100 uF capacitor across HX711 `VCC` and `GND` if readings are noisy.

## Firmware Pin Reference

| Firmware symbol | Arduino pin | Hardware |
| --- | --- | --- |
| `PIN_STEP` | D2 | CL86T `PUL+` |
| `PIN_DIR` | D3 | CL86T `DIR+` |
| `PIN_EN` | D4 | Reserved for optional future `ENA+` control |
| `PIN_FSR_1` | A0 | DF9-16 FSR divider output |
| `PIN_HX711_1_DT` | D5 | HX711 #1 data |
| `PIN_HX711_1_SCK` | D6 | HX711 #1 clock |
| `PIN_HX711_2_DT` | D7 | HX711 #2 data |
| `PIN_HX711_2_SCK` | D8 | HX711 #2 clock |

`PIN_EN` exists in firmware, but the physical `ENA+` wire should stay off Arduino Pin 4 during bring-up.

## First Power-On

Before applying power:

- Power supply AC input is wired, but not plugged in yet.
- Power supply DC output goes to the two CL86T `AC` input terminals.
- Motor phases are connected: black/green/red/blue to `A+`/`A-`/`B+`/`B-`.
- Encoder is connected to `EA+`, `EA-`, `EB+`, `EB-`, `VCC`, and `EGND`.
- Arduino D2 goes to `PUL+`.
- Arduino D3 goes to `DIR+`.
- Arduino GND goes to `PUL-`, `DIR-`, and `ENA-`.
- Arduino D4 is not connected to `ENA+`.
- CL86T input selector is set to `5V`.
- DIP switches and S1 are set for initial bring-up.
- Motor shaft is free to rotate.

Power-on sequence:

1. Connect Arduino USB.
2. Upload firmware with PlatformIO.
3. Open serial monitor at 115200 baud.
4. Apply power to the LE-350-60.
5. Confirm the CL86T green LED is on and red alarm LED is off.
6. Send `?` to verify serial communication.
7. Send `n` to nudge one step.
8. Send `m`, enter an angle such as `90`, then send `t` to test rotate-and-return.

## Serial Commands

| Command | Meaning |
| --- | --- |
| `S` / `s` | Stop motor |
| `H` / `h` | Move to home position |
| `0` | Set current position as home |
| `+` | Increase speed by 100 steps/sec |
| `-` | Decrease speed by 100 steps/sec |
| `?` | Show status |
| `N` / `n` | Nudge one step |
| `M` / `m` | Map an angle for trigger |
| `T` / `t` | Trigger mapped angle and return |
| `I` / `i` | Invert direction in software |
| `P` / `p` | Read FSR value on A0 |
| `L` / `l` | Read load sensor values |
| `C` / `c` | Calibrate sensors |
| `D` / `d` | Continuous A0 diagnostic mode |
| `A` / `a` | Toggle FSR auto-trigger |

## Troubleshooting

### Motor Does Not Move

1. Confirm CL86T green LED is on and red alarm LED is off.
2. Disconnect Arduino Pin 4 from `ENA+` if present.
3. Keep `ENA-` connected to Arduino GND.
4. Confirm D2 -> `PUL+`, D3 -> `DIR+`, and GND -> `PUL-`/`DIR-`/`ENA-`.
5. Confirm the CL86T input selector is set to `5V`.
6. Confirm SW7 is `OFF` for step/direction mode.
7. Try S1 position `4` before using `A`.
8. Verify motor phase wiring.

### Motor Vibrates But Does Not Rotate

- Swap `A+` and `A-`, or swap `B+` and `B-`.
- Confirm the two motor phases are paired correctly with a multimeter.
- Confirm encoder wiring if using closed-loop mode.

### Motor Moves in the Wrong Direction

- Send `i` to invert direction in software.
- Or change `DIR_INVERT` in `include/pins.h`.
- Or toggle SW5 if you want to change the driver's direction setting.

### Direction Never Changes

- Confirm D3 -> `DIR+` and Arduino GND -> `DIR-`.
- Check for loose `DIR+`/`DIR-` terminal connections before changing firmware.
- Confirm SW7 is `OFF` for PUL/DIR single-pulse mode.

### FSR Readings Are Noisy or Wrong

- Confirm the circuit is `5V -> FSR -> A0 -> 10k -> GND`.
- Confirm the resistor is 10 kOhm, not 1 MOhm.
- A calibrated idle baseline around a few hundred is usable if it is stable; the firmware triggers from delta relative to the baseline, not from raw value alone.
- If the baseline is stable but high, calibrate with `C` while the sensor is not pressed and use `P` to confirm the delta stays close to 0 at idle.
- Use `D` to watch A0 continuously.
- Keep FSR wires away from motor and power wiring.
- Add decoupling on Arduino 5V/GND if needed.

### HX711 Reads Not Ready or Noisy

- Check `DT` and `SCK` wiring for each module.
- Confirm HX711 `VCC` is connected to Arduino 5V and `GND` to Arduino GND.
- Let the module stabilize for 1-2 seconds after power-on.
- Add a 100 uF capacitor across HX711 power pins if needed.

### Driver or Motor Runs Hot

- Start with S1 position `4`.
- Mount the CL86T vertically with airflow around the heat sink.
- Add a cooling fan if the driver approaches 35-40C.
- Reduce duty cycle or current if temperatures keep rising.

## Optional Connections

Optional CL86T outputs are not needed for first motion:

- `ALM+` / `ALM-`: alarm/fault output, max 30V DC / 100mA.
- `BRK/PEND+` / `BRK/PEND-`: brake or pend output, max 30V DC / 100mA; use a relay for a brake.
- EMI filter: useful between power supply and driver in noisy environments.
- Driven `ENA+`: add only after baseline motion works and enable polarity is verified.

## Maintenance

- Inspect terminal screws and insulation monthly.
- Keep driver, power supply, and motor ventilated.
- Keep signal/sensor wiring separated from motor power wiring.
- Re-run `C` calibration after changing sensor mounting or wiring.
