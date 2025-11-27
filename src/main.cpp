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
 *
 * Serial Commands:
 *   'F' or 'f' - Move forward (CW)
 *   'B' or 'b' - Move backward (CCW)
 *   'S' or 's' - Stop motor
 *   'H' or 'h' - Move to home position (0 steps)
 *   '+' - Increase speed
 *   '-' - Decrease speed
 *   '?' - Show current status
 *   'R' or 'r' - Rotate one full revolution
 *   'N' or 'n' - Rotate N steps (format: N100 for 100 steps)
 *   'A' or 'a' - Rotate N degrees (format: A90 for 90 degrees)
 */

#include <Arduino.h>
#include <AccelStepper.h>
#include "pins.h"

// ---- Motor Object ----
AccelStepper motor(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);

// ---- State Variables ----
float currentSpeed = SPEED_DEFAULT_SPS;
bool motorEnabled = true;

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
  Serial.println(F("===================\n"));
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
 * Process serial commands
 */
void processSerialCommand()
{
  if (!Serial.available())
    return;

  char cmd = Serial.read();

  // Clear any remaining characters in buffer
  while (Serial.available())
    Serial.read();

  switch (cmd)
  {
    case 'F':
    case 'f':
      // Move forward (clockwise)
      Serial.println(F("Moving forward..."));
      motor.setMaxSpeed(currentSpeed);
      motor.move(1000); // Move 1000 steps forward
      break;

    case 'B':
    case 'b':
      // Move backward (counter-clockwise)
      Serial.println(F("Moving backward..."));
      motor.setMaxSpeed(currentSpeed);
      motor.move(-1000); // Move 1000 steps backward
      break;

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

    case 'R':
    case 'r':
      // Rotate one full revolution
      Serial.println(F("Rotating one full revolution..."));
      motor.setMaxSpeed(currentSpeed);
      motor.move(STEPS_PER_REV);
      break;

    case 'N':
    case 'n':
      // Rotate N steps (wait for number)
      Serial.println(F("Enter number of steps (e.g., 100):"));
      while (!Serial.available())
        motor.run(); // Keep motor running while waiting

      if (Serial.available())
      {
        long steps = Serial.parseInt();
        Serial.print(F("Moving "));
        Serial.print(steps);
        Serial.println(F(" steps..."));
        motor.setMaxSpeed(currentSpeed);
        motor.move(steps);
      }
      break;

    case 'A':
    case 'a':
      // Rotate N degrees (wait for number)
      Serial.println(F("Enter number of degrees (e.g., 90):"));
      while (!Serial.available())
        motor.run(); // Keep motor running while waiting

      if (Serial.available())
      {
        float degrees = Serial.parseFloat();
        long steps = degToSteps(degrees);
        Serial.print(F("Moving "));
        Serial.print(degrees);
        Serial.print(F(" degrees ("));
        Serial.print(steps);
        Serial.println(F(" steps)..."));
        motor.setMaxSpeed(currentSpeed);
        motor.move(steps);
      }
      break;

    case 'E':
    case 'e':
      // Toggle enable
      motorEnabled = !motorEnabled;
      setMotorEnabled(motorEnabled);
      Serial.print(F("Motor "));
      Serial.println(motorEnabled ? F("ENABLED") : F("DISABLED"));
      break;

    default:
      Serial.println(F("\n=== Available Commands ==="));
      Serial.println(F("F/f - Move forward (1000 steps)"));
      Serial.println(F("B/b - Move backward (1000 steps)"));
      Serial.println(F("S/s - Stop motor"));
      Serial.println(F("H/h - Move to home (position 0)"));
      Serial.println(F("+   - Increase speed by 100 steps/sec"));
      Serial.println(F("-   - Decrease speed by 100 steps/sec"));
      Serial.println(F("?   - Show status"));
      Serial.println(F("R/r - Rotate one full revolution"));
      Serial.println(F("N/n - Rotate N steps (prompts for number)"));
      Serial.println(F("A/a - Rotate N degrees (prompts for number)"));
      Serial.println(F("E/e - Toggle motor enable"));
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

  // Configure pins
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_EN, OUTPUT);

  // Initialize motor
  motor.setPinsInverted(DIR_INVERT, false, EN_ACTIVE_LOW);
  motor.setMinPulseWidth(3); // Minimum pulse width in microseconds
  motor.setAcceleration(ACCEL_SPS2);
  motor.setMaxSpeed(currentSpeed);
  motor.setCurrentPosition(0);

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
  Serial.println(F("\nType '?' for help or any command to start"));
  Serial.println(F("========================================\n"));
}

// ---- Main Loop ----
void loop()
{
  // Process serial commands
  processSerialCommand();

  // Run motor (non-blocking)
  motor.run();

  // Small delay to prevent overwhelming the serial buffer
  delayMicroseconds(100);
}

