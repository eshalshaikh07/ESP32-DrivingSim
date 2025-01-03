#include <BleKeyboard.h>  // Include the BLE Keyboard library

// BLE Keyboard setup
BleKeyboard bleKeyboard("ESP32 Steering", "Eshal's Setup", 100);

// Pin Definitions for Buttons
#define ACCELERATOR_PIN 21  // Accelerator button pin (GPIO21)
#define BRAKE_PIN 22        // Brake button pin (GPIO22)
#define ENCODER_PIN_CLK 32    // Rotary Encoder CLK pin
#define ENCODER_PIN_DT  33    // Rotary Encoder DT pin
#define ENCODER_PIN_SW  25    // Rotary Encoder Button (SW) pin

// Button press states
bool isAcceleratorPressed = false;
bool isBrakePressed = false;

// Rotary Encoder state
int lastEncoderPosition = 0;  // Last known encoder position
int encoderPosition = 0;      // Current rotary encoder position
bool lastCLKState = HIGH;     // Last state of CLK pin

// Debounce variables for buttons
unsigned long lastDebounceTime = 0;  // Last time the button state was updated
unsigned long debounceDelay = 50;    // Debounce delay (50 ms)

void setup() {
  Serial.begin(115200);  // Start Serial Monitor

  // Set up button pins as input with internal pull-up resistors
  pinMode(ACCELERATOR_PIN, INPUT_PULLUP);  // Pull-up resistor for accelerator button
  pinMode(BRAKE_PIN, INPUT_PULLUP);        // Pull-up resistor for brake button

  // Set up the rotary encoder pins as inputs
  pinMode(ENCODER_PIN_CLK, INPUT_PULLUP);
  pinMode(ENCODER_PIN_DT, INPUT_PULLUP);
  pinMode(ENCODER_PIN_SW, INPUT_PULLUP);

  // Initialize BLE Keyboard
  Serial.println("Starting BLE Keyboard...");
  bleKeyboard.begin();
}

void loop() {
  if (bleKeyboard.isConnected()) {  // Check if BLE keyboard is connected

    // Handle Accelerator and Brake Buttons
    handleButtons();

    // Read the current rotary encoder position
    encoderPosition = readEncoder();

    // If there's any change in the encoder position, adjust steering
    if (encoderPosition != lastEncoderPosition) {
      if (encoderPosition > lastEncoderPosition) {
        // Rotating right: keep the key pressed
        Serial.println("Steering Right");
        bleKeyboard.press(KEY_RIGHT_ARROW);  // Keep pressing right key
      } else if (encoderPosition < lastEncoderPosition) {
        // Rotating left: keep the key pressed
        Serial.println("Steering Left");
        bleKeyboard.press(KEY_LEFT_ARROW);  // Keep pressing left key
      }
    }

    // Release the key when the knob is back to neutral position
    if (encoderPosition == 0) {
      bleKeyboard.release(KEY_LEFT_ARROW);
      bleKeyboard.release(KEY_RIGHT_ARROW);
    }

    // Store the current encoder position for the next loop
    lastEncoderPosition = encoderPosition;

    delay(10);  // Small delay for smooth operation
  }
}

// Function to handle the Accelerator and Brake Buttons
void handleButtons() {
  // Read the current state of the accelerator and brake buttons (LOW means pressed)
  bool currentAcceleratorState = digitalRead(ACCELERATOR_PIN) == LOW;
  bool currentBrakeState = digitalRead(BRAKE_PIN) == LOW;

  // Check if enough time has passed to debounce the buttons
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Debounce logic for accelerator button press/release
    if (currentAcceleratorState && !isAcceleratorPressed) {
      Serial.println("Accelerator Pressed");
      bleKeyboard.press(KEY_UP_ARROW);  // Send "Up Arrow" key press (for accelerating)
      isAcceleratorPressed = true;
    } else if (!currentAcceleratorState && isAcceleratorPressed) {
      Serial.println("Accelerator Released");
      bleKeyboard.release(KEY_UP_ARROW);  // Release "Up Arrow" key (when released)
      isAcceleratorPressed = false;
    }

    // Debounce logic for brake button press/release
    if (currentBrakeState && !isBrakePressed) {
      Serial.println("Brake Pressed");
      bleKeyboard.press(KEY_DOWN_ARROW);  // Send "Down Arrow" key press (for braking)
      isBrakePressed = true;
    } else if (!currentBrakeState && isBrakePressed) {
      Serial.println("Brake Released");
      bleKeyboard.release(KEY_DOWN_ARROW);  // Release "Down Arrow" key (when released)
      isBrakePressed = false;
    }

    // Update the debounce time
    lastDebounceTime = millis();
  }

  // If the accelerator button is pressed, keep sending the "UP arrow" key
  if (isAcceleratorPressed) {
    Serial.println("Accelerating...");
    bleKeyboard.press(KEY_UP_ARROW);  // Continue pressing the "Up Arrow" key
  } else {
    bleKeyboard.release(KEY_UP_ARROW);  // Release the "Up Arrow" key if button is not pressed
  }

  // If the brake button is pressed, keep sending the "DOWN arrow" key
  if (isBrakePressed) {
    Serial.println("Braking...");
    bleKeyboard.press(KEY_DOWN_ARROW);  // Continue pressing the "Down Arrow" key
  } else {
    bleKeyboard.release(KEY_DOWN_ARROW);  // Release the "Down Arrow" key if button is not pressed
  }
}

// Function to read the rotary encoder position
int readEncoder() {
  static bool lastCLKState = digitalRead(ENCODER_PIN_CLK);
  static int position = 0;

  bool currentCLKState = digitalRead(ENCODER_PIN_CLK);
  bool currentDTState = digitalRead(ENCODER_PIN_DT);

  if (currentCLKState != lastCLKState && currentCLKState == LOW) {
    if (currentDTState != currentCLKState) {
      position++;  // Clockwise rotation (right)
    } else {
      position--;  // Counter-clockwise rotation (left)
    }
  }

  lastCLKState = currentCLKState;
  return position;
}
