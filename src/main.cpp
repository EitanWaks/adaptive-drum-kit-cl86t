/**
 * Adaptive Drum Kit - CL86T Motor Control
 *
 * Basic motor control for 34E1K-120 stepper motor via CL86T closed-loop driver.
 * This firmware provides serial command interface for computer control.
 *
 * Hardware:
 *   - Stepper Motor: 34E1K-120 (NEMA 34, 200 steps/rev)
 *   - Driver: CL86T (Closed-loop stepper driver)
 *   - Power Supply: LE-350-60
 *   - Microcontroller: Arduino UNO R3
 *   - Force-Sensitive Resistor (FSR): DF9-16 sensor on A0
 *   - Load Sensors: Up to 2 HX711 modules with strain gauge load cells
 *
 * Serial Commands:
 *   'S' or 's' - Stop motor
 *   'H' or 'h' - Move to home position (0 steps)
 *   '0' - Set current position as home (zeroing home)
 *   '+' - Increase speed
 *   '-' - Decrease speed
 *   '?' - Show current status
 *   'N' or 'n' - Nudge (move 1 step)
 *   'M' or 'm' - Map/calibrate rotating N degrees (stores angle for trigger)
 *   'T' or 't' - Trigger (rotate stored angle CW then return CCW)
 *   'I' or 'i' - Inverse (reverse clockwise/counterclockwise direction)
 *   'P' or 'p' - Read FSR sensor values
 *   'L' or 'l' - Read load sensor values
 *   'C' or 'c' - Calibrate all sensors
 *   'D' or 'd' - Diagnostic mode (continuous A0 reading)
 *   'A' or 'a' - Toggle FSR auto-trigger mode
 */

#include <Arduino.h>
#include <AccelStepper.h>
#include <HX711.h>
#include "pins.h"

// ---- Motor Object ----
AccelStepper motor(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);

// ---- Load Sensor Objects (HX711) ----
HX711 loadSensor1;
HX711 loadSensor2;

// ---- State Variables ----
float currentSpeed = SPEED_DEFAULT_SPS;
bool motorEnabled = true;
bool commandDirectionInverted = false;
long returnToPosition = 0;  // For rotate-and-return command
bool waitingForReturn = false;  // Flag to track if we're in return phase
bool inReturnPhase = false;  // Flag to track if we're currently returning (not going forward)
float mappedAngle = 0.0f;  // Stored angle from map command (in degrees)

// ---- FSR Auto-Trigger Settings ----
bool autoTriggerEnabled = false;  // Enable/disable automatic motor triggering from FSR
unsigned long lastFSRTriggerTime = 0;  // Debounce timer
const unsigned long FSR_DEBOUNCE_MS = 200;  // Minimum time between triggers (200ms)
bool fsrWasPressed = false;  // Track previous state for edge detection

// ---- Sensor Calibration Values ----
// These should be calibrated for your specific sensors
// For load sensors: Set to zero reading when no load is applied
long loadSensor1Zero = 0;
long loadSensor2Zero = 0;
// For FSR sensors: Baseline reading when no pressure
int fsrBaseline[NUM_FSR_SENSORS] = {512};  // Default to mid-point (2.5V)
// Pressure detection threshold (adjusted value must exceed this to register as pressure)
int fsrThreshold[NUM_FSR_SENSORS] = {50};  // Default threshold
// Number of samples for averaging (reduces noise)
static const uint8_t FSR_AVG_SAMPLES = 10;

// ---- Utility Functions ----
/**
 * Convert degrees to steps
 */
static inline long degToSteps(float deg)
{
  const float stepsPerRev = (float)STEPS_PER_REV * GEAR_RATIO;
  return (long)((deg / 360.0f) * stepsPerRev);
}

/**
 * Apply the runtime inverse command to a relative move.
 * DIR_INVERT remains a fixed hardware polarity setting in pins.h.
 */
static inline long applyCommandDirection(long steps)
{
  return commandDirectionInverted ? -steps : steps;
}

/**
 * Print current status to serial
 */
void printStatus()
{
  Serial.println(F("\n=== Motor Status ==="));
  Serial.print(F("Speed: "));
  Serial.print(currentSpeed);
  Serial.println(F(" steps/sec"));
  Serial.print(F("Position: "));
  Serial.print(motor.currentPosition());
  Serial.println(F(" steps"));
  Serial.print(F("Target: "));
  Serial.print(motor.targetPosition());
  Serial.println(F(" steps"));
  Serial.print(F("Distance to go: "));
  Serial.print(motor.distanceToGo());
  Serial.println(F(" steps"));
  Serial.print(F("Motor enabled: "));
  Serial.println(motorEnabled ? F("YES") : F("NO"));
  Serial.print(F("Command direction: "));
  Serial.println(commandDirectionInverted ? F("INVERTED") : F("NORMAL"));
  Serial.print(F("DIR pin polarity inverted: "));
  Serial.println(DIR_INVERT ? F("YES") : F("NO"));
  Serial.println(F("===================\n"));
}

/**
 * Read a single FSR sensor with averaging (reduces noise)
 */
int readFSRSensorAveraged(uint8_t pin)
{
  long sum = 0;
  for (uint8_t i = 0; i < FSR_AVG_SAMPLES; i++)
  {
    sum += analogRead(pin);
    delayMicroseconds(100);  // Small delay between samples
  }
  return (int)(sum / FSR_AVG_SAMPLES);
}

/**
 * Read all FSR sensor values with noise filtering
 * DEBUG MODE: Only reading A0 (FSR sensor)
 */
void readFSRSensors()
{
  Serial.println(F("\n=== Force Sensor (A0 Only - Debug Mode) ==="));

  // Read averaged value to reduce noise
  int rawValue = readFSRSensorAveraged(PIN_FSR_1);
  int adjustedValue = rawValue - fsrBaseline[0];
  float voltage = (rawValue / 1023.0) * 5.0;  // Convert to voltage (0-5V)

  // Check for pressure (absolute value of adjusted reading exceeds threshold)
  bool pressureDetected = abs(adjustedValue) > fsrThreshold[0];

  // Calculate estimated resistance (for FSR debugging)
  // Assuming 10kΩ pull-down resistor and sensor between 5V and A0
  // Voltage divider: Vout = Vin * (R2 / (R1 + R2))
  // Solving for R1 (FSR resistance): R1 = R2 * (Vin/Vout - 1)
  // Where R2 = 10000Ω (10kΩ), Vin = 5V, Vout = voltage
  float estimatedResistance = 0.0;
  if (voltage > 0.01 && voltage < 4.99)  // Avoid division by zero
  {
    float r2 = 10000.0;  // 10kΩ pull-down resistor
    estimatedResistance = r2 * ((5.0 / voltage) - 1.0);
  }

  // Diagnostic: Check if reading makes sense
  const char* status = "";
  if (rawValue < 50)
  {
    status = " [OK: Not pressed]";
  }
  else if (rawValue > 200 && rawValue < 800)
  {
    status = " [WARNING: Unexpected, should be <50 or >800]";
  }
  else if (rawValue >= 800)
  {
    status = " [OK: Pressed]";
  }

  Serial.print(F("Sensor A0: Raw="));
  Serial.print(rawValue);
  Serial.print(F(" ("));
  Serial.print(voltage, 2);
  Serial.print(F("V), Adjusted="));
  Serial.print(adjustedValue);
  Serial.print(F(", Threshold="));
  Serial.print(fsrThreshold[0]);
  Serial.print(F(", Est. Resistance="));
  if (estimatedResistance > 0)
  {
    if (estimatedResistance > 1000000)
    {
      Serial.print(estimatedResistance / 1000000.0, 2);
      Serial.print(F(" MΩ"));
    }
    else if (estimatedResistance > 1000)
    {
      Serial.print(estimatedResistance / 1000.0, 2);
      Serial.print(F(" kΩ"));
    }
    else
    {
      Serial.print(estimatedResistance, 0);
      Serial.print(F(" Ω"));
    }
  }
  else
  {
    Serial.print(F("N/A (maxed out)"));
  }
  Serial.print(status);
  if (pressureDetected)
  {
    Serial.print(F(" [PRESSURE DETECTED!]"));
  }
  else if (rawValue > 200)
  {
    Serial.print(F(" [Above threshold but not detected - threshold too high?]"));
  }
  Serial.println();
  Serial.println(F("==========================================\n"));
}

/**
 * Read all load sensor values
 */
void readLoadSensors()
{
  Serial.println(F("\n=== Load Sensors ==="));

  // Read load sensor 1
  if (loadSensor1.is_ready())
  {
    long rawValue1 = loadSensor1.read();
    long adjustedValue1 = rawValue1 - loadSensor1Zero;
    Serial.print(F("Load Sensor 1: Raw="));
    Serial.print(rawValue1);
    Serial.print(F(", Adjusted="));
    Serial.print(adjustedValue1);
    Serial.println();
  }
  else
  {
    Serial.println(F("Load Sensor 1: Not ready"));
  }

  // Read load sensor 2
  if (loadSensor2.is_ready())
  {
    long rawValue2 = loadSensor2.read();
    long adjustedValue2 = rawValue2 - loadSensor2Zero;
    Serial.print(F("Load Sensor 2: Raw="));
    Serial.print(rawValue2);
    Serial.print(F(", Adjusted="));
    Serial.print(adjustedValue2);
    Serial.println();
  }
  else
  {
    Serial.println(F("Load Sensor 2: Not ready"));
  }

  Serial.println(F("====================\n"));
}

/**
 * Calibrate load sensors (set zero point)
 */
void calibrateLoadSensors()
{
  Serial.println(F("Calibrating load sensors (no load)..."));

  if (loadSensor1.is_ready())
  {
    loadSensor1Zero = loadSensor1.read();
    Serial.print(F("Load Sensor 1 zero: "));
    Serial.println(loadSensor1Zero);
  }

  if (loadSensor2.is_ready())
  {
    loadSensor2Zero = loadSensor2.read();
    Serial.print(F("Load Sensor 2 zero: "));
    Serial.println(loadSensor2Zero);
  }

  Serial.println(F("Calibration complete."));
}

/**
 * Calibrate FSR sensor - set baseline when no pressure
 * Takes many samples over a longer period to account for noise
 */
void calibrateFSRSensors()
{
  Serial.println(F("Calibrating FSR sensor (A0 only - Debug Mode)..."));
  Serial.println(F("Please ensure NO PRESSURE is applied to sensor..."));
  delay(500);  // Wait for any transient signals to settle

  // Take many readings over a longer period and average
  // This helps account for electrical noise
  const int CALIBRATION_SAMPLES = 50;
  const int CALIBRATION_DELAY_MS = 20;

  long sum = 0;
  int minVal = 1023;
  int maxVal = 0;

  for (int j = 0; j < CALIBRATION_SAMPLES; j++)
  {
    int reading = analogRead(PIN_FSR_1);
    sum += reading;
    if (reading < minVal) minVal = reading;
    if (reading > maxVal) maxVal = reading;
    delay(CALIBRATION_DELAY_MS);
  }

  fsrBaseline[0] = sum / CALIBRATION_SAMPLES;

  // Calculate noise level (peak-to-peak variation)
  int noiseLevel = maxVal - minVal;

  // Set threshold based on noise level
  // For FSR: threshold should be reasonable (not too high)
  // If noise is very high, use a fixed reasonable threshold
  if (noiseLevel > 200)
  {
    // High noise - use fixed threshold (100 raw units = ~0.5V change)
    fsrThreshold[0] = 100;
    Serial.println(F("Warning: High noise detected. Using fixed threshold of 100."));
  }
  else
  {
    // Normal noise - use 3x noise level, but cap at reasonable maximum
    fsrThreshold[0] = (noiseLevel * 3 > 200) ? 200 : (noiseLevel * 3 < 30 ? 30 : noiseLevel * 3);
  }

  Serial.print(F("Sensor A0: Baseline="));
  Serial.print(fsrBaseline[0]);
  Serial.print(F(" ("));
  Serial.print((fsrBaseline[0] / 1023.0) * 5.0, 2);
  Serial.print(F("V), Noise="));
  Serial.print(noiseLevel);
  Serial.print(F(" ("));
  Serial.print(minVal);
  Serial.print(F("-"));
  Serial.print(maxVal);
  Serial.print(F("), Threshold="));
  Serial.println(fsrThreshold[0]);

  Serial.println(F("Calibration complete."));

  // Diagnostic information
  if (noiseLevel > 200)
  {
    Serial.println(F("WARNING: Very high noise detected!"));
    Serial.println(F("   - Check wiring connections"));
    Serial.println(F("   - Verify 10kΩ resistor is correct value"));
    Serial.println(F("   - Ensure FSR is properly connected"));
    Serial.println(F("   - Check for loose wires or shorts"));
  }
  else if (noiseLevel > 100)
  {
    Serial.println(F("CAUTION: High noise level detected."));
    Serial.println(F("   - Wiring may need improvement"));
  }

  // Check if baseline is reasonable
  if (fsrBaseline[0] > 200)
  {
    Serial.print(F("WARNING: Baseline is high ("));
    Serial.print(fsrBaseline[0]);
    Serial.println(F(")."));
    Serial.println(F("   Expected: 0-100 when FSR not pressed"));
    Serial.println(F("   Possible issues:"));
    Serial.println(F("   - FSR may be partially pressed"));
    Serial.println(F("   - Wiring may be incorrect"));
    Serial.println(F("   - FSR may be damaged"));
  }

  Serial.println(F("\nExpected behavior:"));
  Serial.println(F("  - Not pressed: Reading 0-100 (near 0V)"));
  Serial.println(F("  - Pressed: Reading increases significantly (200-1023)"));
  Serial.print(F("  - Threshold set to: "));
  Serial.println(fsrThreshold[0]);
}

/**
 * Enable or disable the motor
 */
void setMotorEnabled(bool enabled)
{
  motorEnabled = enabled;
  digitalWrite(PIN_EN, enabled ? (EN_ACTIVE_LOW ? LOW : HIGH) : (EN_ACTIVE_LOW ? HIGH : LOW));
}

/**
 * Set motor speed
 */
void setSpeed(float speed)
{
  speed = constrain(speed, SPEED_MIN_SPS, SPEED_MAX_SPS);
  currentSpeed = speed;
  motor.setMaxSpeed(currentSpeed);
  Serial.print(F("Speed set to: "));
  Serial.print(currentSpeed);
  Serial.println(F(" steps/sec"));
}

/**
 * Start a mapped rotate-and-return action.
 */
void startTriggerMove(const __FlashStringHelper* sourceLabel)
{
  long rawSteps = degToSteps(mappedAngle);
  long moveSteps = applyCommandDirection(rawSteps);
  returnToPosition = motor.currentPosition();
  inReturnPhase = false;

  Serial.print(sourceLabel);
  Serial.print(F(": Rotating "));
  Serial.print(mappedAngle);
  Serial.print(F(" degrees ("));
  Serial.print(moveSteps);
  Serial.print(F(" commanded steps) from position "));
  Serial.print(returnToPosition);
  Serial.println(F(", then returning..."));

  motor.setMaxSpeed(currentSpeed);
  motor.move(moveSteps);
  waitingForReturn = true;
}

/**
 * Process serial commands
 */
void processSerialCommand()
{
  if (!Serial.available())
    return;

  char cmd = Serial.read();

  // Clear any remaining characters in buffer (like newline after command)
  while (Serial.available() && (Serial.peek() == '\n' || Serial.peek() == '\r'))
    Serial.read();

  // Clear any other remaining characters
  while (Serial.available())
    Serial.read();

  // Debug: print received command (can be removed later)
  // Serial.print(F("Received command: "));
  // Serial.println(cmd);

  switch (cmd)
  {
    case 'S':
    case 's':
      // Stop motor
      Serial.println(F("Stopping motor..."));
      motor.stop();
      motor.setCurrentPosition(motor.currentPosition());
      break;

    case 'H':
    case 'h':
      // Move to home (position 0)
      Serial.println(F("Moving to home position..."));
      motor.setMaxSpeed(currentSpeed);
      motor.moveTo(0);
      waitingForReturn = false;  // Cancel any pending return
      inReturnPhase = false;  // Reset return phase
      break;

    case '0':
      // Set current position as home (zeroing home)
      motor.setCurrentPosition(0);
      Serial.println(F("Current position set as home (0)"));
      waitingForReturn = false;  // Cancel any pending return
      inReturnPhase = false;  // Reset return phase
      break;

    case '+':
      // Increase speed
      setSpeed(currentSpeed + 100.0f);
      break;

    case '-':
      // Decrease speed
      setSpeed(currentSpeed - 100.0f);
      break;

    case '?':
      // Show status
      printStatus();
      break;

    case 'N':
    case 'n':
      // Nudge (move 1 step)
      motor.setMaxSpeed(currentSpeed);
      motor.move(applyCommandDirection(1));
      waitingForReturn = false;  // Cancel any pending return
      inReturnPhase = false;  // Reset return phase
      break;

    case 'M':
    case 'm':
    {
      // Map/calibrate rotating N degrees (stores angle for trigger)
      Serial.println(F("Enter number of degrees to map (e.g., 90):"));

      // Read input until Enter/Return is pressed
      String inputString = "";
      while (true)
      {
        motor.run(); // Keep motor running while waiting

        if (Serial.available())
        {
          char inChar = Serial.read();

          // Check for Enter/Return (newline or carriage return)
          if (inChar == '\n' || inChar == '\r')
          {
            if (inputString.length() > 0)
            {
              Serial.println(); // Echo the newline
              break; // Got the input, exit loop
            }
            // If empty, continue waiting
          }
          else if ((inChar >= '0' && inChar <= '9') || inChar == '.' || inChar == '-' || inChar == '+')
          {
            // Valid character for number input - echo it back
            inputString += inChar;
            Serial.print(inChar); // Echo the character so user can see what they're typing
          }
          // Ignore other characters
        }
      }

      // Parse the input string
      if (inputString.length() > 0)
      {
        float degrees = inputString.toFloat();
        mappedAngle = degrees;  // Store the angle
        long steps = degToSteps(degrees);
        Serial.print(F("Mapping rotation of "));
        Serial.print(degrees);
        Serial.print(F(" degrees ("));
        Serial.print(steps);
        Serial.print(F(" steps) - stored for trigger (no movement)"));
        Serial.println();
      }
      break;
    }

    case 'T':
    case 't':
      // Trigger (rotate stored angle CW then return CCW)
      if (mappedAngle == 0.0f)
      {
        Serial.println(F("Error: No angle mapped. Use M command first to set angle."));
        break;
      }
      // Ignore if a trigger is already in progress to prevent creeping
      if (waitingForReturn)
      {
        Serial.println(F("Trigger already in progress. Wait for completion."));
        break;
      }
      {
        startTriggerMove(F("Trigger"));
      }
      break;

    case 'I':
    case 'i':
      // Reverse commanded motion direction without changing hardware pin polarity.
      commandDirectionInverted = !commandDirectionInverted;
      Serial.print(F("Inverse: Command direction "));
      Serial.println(commandDirectionInverted ? F("INVERTED") : F("NORMAL"));
      break;

    case 'P':
    case 'p':
      // Read FSR sensor values
      readFSRSensors();
      break;

    case 'L':
    case 'l':
      // Read load sensor values
      readLoadSensors();
      break;

    case 'C':
    case 'c':
      // Calibrate sensors
      Serial.println(F("Calibrating all sensors..."));
      calibrateFSRSensors();
      calibrateLoadSensors();
      break;

    case 'A':
    case 'a':
      // Toggle auto-trigger mode (motor triggers automatically on FSR pressure)
      autoTriggerEnabled = !autoTriggerEnabled;
      Serial.print(F("Auto-trigger: "));
      if (autoTriggerEnabled)
      {
        Serial.println(F("ENABLED"));
        if (mappedAngle == 0.0f)
        {
          Serial.println(F("Warning: No angle mapped. Use 'M' command first."));
        }
        else
        {
          Serial.print(F("Motor will trigger on FSR pressure ("));
          Serial.print(mappedAngle);
          Serial.println(F(" degrees)"));
        }
      }
      else
      {
        Serial.println(F("DISABLED"));
      }
      break;

    case 'D':
    case 'd':
      // Diagnostic mode - continuous reading to check for wiring issues
      {
        Serial.println(F("Diagnostic mode - reading A0 continuously..."));
        Serial.println(F("Press any key to exit"));

        while (!Serial.available())
        {
          int rawValue = analogRead(PIN_FSR_1);
          float voltage = (rawValue / 1023.0) * 5.0;
          Serial.print(F("A0: Raw="));
          Serial.print(rawValue);
          Serial.print(F(" ("));
          Serial.print(voltage, 2);
          Serial.print(F("V)"));
          Serial.println();
          delay(100);
        }
        Serial.read();  // Clear the character
        Serial.println(F("Diagnostic mode exited."));
      }
      break;

    default:
      Serial.println(F("\n=== Available Commands ==="));
      Serial.println(F("S/s - Stop motor"));
      Serial.println(F("H/h - Home (move to home position)"));
      Serial.println(F("0   - Zeroing home (set current position as home)"));
      Serial.println(F("+   - Increase speed by 100 steps/sec"));
      Serial.println(F("-   - Decrease speed by 100 steps/sec"));
      Serial.println(F("?   - Show status"));
      Serial.println(F("N/n - Nudge (move 1 step)"));
      Serial.println(F("M/m - Map (set angle for trigger, prompts for degrees)"));
      Serial.println(F("T/t - Trigger (rotate stored angle CW then return CCW)"));
      Serial.println(F("I/i - Inverse (reverse clockwise/counterclockwise)"));
      Serial.println(F("P/p - Read FSR sensor values"));
      Serial.println(F("L/l - Read load sensor values"));
      Serial.println(F("C/c - Calibrate all sensors (set zero/baseline)"));
      Serial.println(F("D/d - Diagnostic mode (continuous sensor reading)"));
      Serial.println(F("A/a - Toggle auto-trigger (motor triggers on FSR pressure)"));
      Serial.println(F("========================"));
      break;
  }
}

// ---- Setup ----
void setup()
{
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial)
    ; // Wait for serial port to connect (needed for some boards)

  delay(1000); // Give time for serial monitor to open

  // Configure motor control pins
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_EN, OUTPUT);

  // Configure FSR sensor pin (analog input) - DEBUG: Only A0
  pinMode(PIN_FSR_1, INPUT);
  // Other pins disabled for debugging
  // pinMode(PIN_FSR_2, INPUT);
  // pinMode(PIN_FSR_3, INPUT);
  // pinMode(PIN_FSR_4, INPUT);
  // pinMode(PIN_FSR_5, INPUT);
  // pinMode(PIN_FSR_6, INPUT);

  // Initialize motor
  motor.setPinsInverted(DIR_INVERT, false, EN_ACTIVE_LOW);
  motor.setMinPulseWidth(3); // Minimum pulse width in microseconds
  motor.setAcceleration(ACCEL_SPS2);
  motor.setMaxSpeed(currentSpeed);
  motor.setCurrentPosition(0);

  // Initialize load sensors (HX711)
  loadSensor1.begin(PIN_HX711_1_DT, PIN_HX711_1_SCK);
  loadSensor2.begin(PIN_HX711_2_DT, PIN_HX711_2_SCK);

  // Set gain for HX711 (128 is default, good for most load cells)
  // Options: 128 (channel A, gain 128), 64 (channel A, gain 64), 32 (channel B, gain 32)
  loadSensor1.set_scale();  // Default scale (1.0)
  loadSensor2.set_scale();  // Default scale (1.0)

  // Tare the load sensors (set zero point)
  // Note: This may take a moment, sensors need to stabilize
  delay(500);
  if (loadSensor1.is_ready())
  {
    loadSensor1Zero = loadSensor1.read();
  }
  if (loadSensor2.is_ready())
  {
    loadSensor2Zero = loadSensor2.read();
  }

  // Enable motor
  setMotorEnabled(true);

  // Print welcome message
  Serial.println(F("\n========================================"));
  Serial.println(F("Adaptive Drum Kit - CL86T Motor Control"));
  Serial.println(F("========================================"));
  Serial.print(F("Motor: 34E1K-120 ("));
  Serial.print(STEPS_PER_REV);
  Serial.println(F(" steps/rev)"));
  Serial.print(F("Driver: CL86T Closed-Loop"));
  Serial.println();
  Serial.print(F("Default speed: "));
  Serial.print(currentSpeed);
  Serial.println(F(" steps/sec"));
  Serial.print(F("FSR sensors: "));
  Serial.print(NUM_FSR_SENSORS);
  Serial.println(F(" configured (A0 only - Debug Mode)"));
  Serial.print(F("Load sensors: "));
  Serial.print(NUM_LOAD_SENSORS);
  Serial.println(F(" configured"));
  Serial.println(F("\nType '?' for help or any command to start"));
  Serial.println(F("Type 'C' to calibrate sensors (recommended on startup)"));
  Serial.println(F("========================================\n"));
}

/**
 * Check FSR sensor and trigger motor if pressure detected
 */
void checkFSRAndTrigger()
{
  if (!autoTriggerEnabled || mappedAngle == 0.0f)
    return;  // Auto-trigger disabled or no angle mapped

  // Read FSR sensor
  int rawValue = readFSRSensorAveraged(PIN_FSR_1);
  int adjustedValue = rawValue - fsrBaseline[0];
  bool pressureDetected = abs(adjustedValue) > fsrThreshold[0];

  // Edge detection: trigger on rising edge (pressure just detected)
  if (pressureDetected && !fsrWasPressed)
  {
    // Check debounce timer
    unsigned long currentTime = millis();
    if (currentTime - lastFSRTriggerTime > FSR_DEBOUNCE_MS)
    {
      // Only trigger if motor is not already moving
      if (!waitingForReturn)
      {
        Serial.print(F("[FSR Trigger] Pressure detected! Rotating "));
        Serial.print(mappedAngle);
        Serial.print(F(" degrees..."));
        Serial.println();

        startTriggerMove(F("[FSR Trigger]"));
        lastFSRTriggerTime = currentTime;
      }
    }
  }

  fsrWasPressed = pressureDetected;
}

// ---- Main Loop ----
void loop()
{
  // Process serial commands
  processSerialCommand();

  // Check FSR sensor and auto-trigger motor if enabled
  checkFSRAndTrigger();

  // Run motor (non-blocking)
  motor.run();

  // Handle rotate-and-return command
  if (waitingForReturn && motor.distanceToGo() == 0)
  {
    if (!inReturnPhase)
    {
      // Motor reached forward target, now return to starting position
      Serial.println(F("Reached target, returning to start position..."));
      motor.setMaxSpeed(currentSpeed);
      motor.moveTo(returnToPosition);
      inReturnPhase = true;  // Now we're in return phase
    }
    else
    {
      // Motor has returned to starting position - complete!
      Serial.println(F("Return complete. Trigger finished."));
      waitingForReturn = false;  // Clear flag, motion complete
      inReturnPhase = false;  // Reset phase flag
    }
  }

  // Small delay to prevent overwhelming the serial buffer
  delayMicroseconds(100);
}

