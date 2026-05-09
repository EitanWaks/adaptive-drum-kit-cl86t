#pragma once
#include <Arduino.h>

/**
 * Pin Configuration for CL86T Closed-Loop Stepper Driver
 *
 * CL86T Driver Signal Connections:
 *   PUL+ -> Arduino STEP pin (via 1kΩ resistor recommended)
 *   PUL- -> Arduino GND
 *   DIR+ -> Arduino DIR pin (via 1kΩ resistor recommended)
 *   DIR- -> Arduino GND
 *   ENA+ -> Leave disconnected from Arduino during initial bring-up
 *           (optional future enable/disable control after polarity is verified)
 *   ENA- -> Arduino GND
 *
 * Note: The CL86T uses opto-isolated inputs. The + terminals are
 * connected to Arduino digital pins, and the - terminals are
 * connected to GND. A 1kΩ resistor in series is recommended
 * but not always required.
 */

// ---- Step/Direction/Enable Pins ----
static const uint8_t PIN_STEP = 2;   // PUL+ on CL86T
static const uint8_t PIN_DIR = 3;    // DIR+ on CL86T
static const uint8_t PIN_EN = 4;     // Reserved for optional future ENA+ control

// ---- Force-Sensitive Resistor (FSR) Sensor Pins ----
// FSR sensors connect to analog pins via voltage divider circuit
// Each sensor needs a 1kΩ resistor in voltage divider configuration
// Maximum 6 sensors on Arduino Uno (A0-A5)
static const uint8_t PIN_FSR_1 = A0;  // FSR sensor 1
static const uint8_t PIN_FSR_2 = A1;  // FSR sensor 2
static const uint8_t PIN_FSR_3 = A2;  // FSR sensor 3
static const uint8_t PIN_FSR_4 = A3;  // FSR sensor 4
static const uint8_t PIN_FSR_5 = A4;  // FSR sensor 5
static const uint8_t PIN_FSR_6 = A5;  // FSR sensor 6

// Number of FSR sensors configured
static const uint8_t NUM_FSR_SENSORS = 1;  // DEBUG: Only A0 for now

// ---- Load Sensor (HX711) Pins ----
// HX711 amplifier module for strain gauge load cells
// Each HX711 module requires 2 digital pins (clock and data)
// Maximum 2 HX711 modules on Arduino Uno (limited by available pins)
static const uint8_t PIN_HX711_1_DT = 5;   // HX711 #1 Data pin
static const uint8_t PIN_HX711_1_SCK = 6;  // HX711 #1 Clock pin
static const uint8_t PIN_HX711_2_DT = 7;   // HX711 #2 Data pin
static const uint8_t PIN_HX711_2_SCK = 8;  // HX711 #2 Clock pin

// Number of load sensors configured
static const uint8_t NUM_LOAD_SENSORS = 2;

// ---- Motor Specifications ----
// 34E1K-120: NEMA 34 stepper motor, 1.8° per step
static const long STEPS_PER_REV = 200;  // Full step mode
static const float GEAR_RATIO = 1.0f;   // Direct drive (adjust if using gearbox)

// ---- Motion Parameters ----
static const float SPEED_MIN_SPS = 100.0f;      // Minimum speed (steps per second)
static const float SPEED_MAX_SPS = 2000.0f;     // Maximum speed (steps per second)
static const float SPEED_DEFAULT_SPS = 500.0f;  // Default speed
static const float ACCEL_SPS2 = 1000.0f;        // Acceleration (steps per second squared)

// ---- Enable Pin Configuration ----
static const bool EN_ACTIVE_LOW = false;  // Set to true if enable is active low
                                          // CL86T typically uses active high

// ---- Direction Configuration ----
static const bool DIR_INVERT = false;     // Set to true to invert direction

