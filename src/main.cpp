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
 */

#include <Arduino.h>
#include <AccelStepper.h>
#include "pins.h"

// ---- Motor Object ----
AccelStepper motor(AccelStepper::DRIVER, PIN_STEP, PIN_DIR);

// ---- State Variables ----
float currentSpeed = SPEED_DEFAULT_SPS;
bool motorEnabled = true;
bool directionInverted = DIR_INVERT;
long returnToPosition = 0;  // For rotate-and-return command
bool waitingForReturn = false;  // Flag to track if we're in return phase
bool inReturnPhase = false;  // Flag to track if we're currently returning (not going forward)
float mappedAngle = 0.0f;  // Stored angle from map command (in degrees)

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
      motor.move(1);
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
        long steps = degToSteps(mappedAngle);
        returnToPosition = motor.currentPosition();
        inReturnPhase = false;  // Start in forward phase
        Serial.print(F("Trigger: Rotating "));
        Serial.print(mappedAngle);
        Serial.print(F(" degrees CW ("));
        Serial.print(steps);
        Serial.print(F(" steps) from position "));
        Serial.print(returnToPosition);
        Serial.println(F(", then returning..."));
        motor.setMaxSpeed(currentSpeed);
        motor.move(steps);
        waitingForReturn = true;  // Set flag to return after reaching target
      }
      break;

    case 'I':
    case 'i':
      // Reverse clockwise/counterclockwise direction
      directionInverted = !directionInverted;
      motor.setPinsInverted(directionInverted, false, EN_ACTIVE_LOW);
      Serial.print(F("Inverse: Direction "));
      Serial.println(directionInverted ? F("INVERTED") : F("NORMAL"));
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
  motor.setPinsInverted(directionInverted, false, EN_ACTIVE_LOW);
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

