#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
const int lockButtonPin = 2;
const int openButtonPin = 3;
const int redLEDPin = 5;
const int greenLEDPin = 6;
const int blueLEDPin = 7; // Optional
const int servoPin = 9;

int doorclosetime = 2000;

// Objects
Servo doorServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address if needed

// State
bool isLocked = true;

void setup() {
  // Initialize Serial
  Serial.begin(9600);

  // Buttons as INPUT_PULLUP
  pinMode(lockButtonPin, INPUT_PULLUP);
  pinMode(openButtonPin, INPUT_PULLUP);

  // RGB LED
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT); // Optional

  // Servo
  doorServo.attach(servoPin);
  doorServo.write(0); // Closed position

  // LCD
  lcd.init();
  lcd.backlight();

  updateStatus(); // Initial display
}

void loop() {
  // Handle lock/unlock toggle
  if (digitalRead(lockButtonPin) == LOW) {
    delay(200); // debounce
    isLocked = !isLocked;
    updateStatus();
    Serial.println(isLocked ? "LOCKED" : "UNLOCKED");
    while (digitalRead(lockButtonPin) == LOW); // wait for release
  }

  // Handle open door
  if (digitalRead(openButtonPin) == LOW && !isLocked) {
    delay(200); // debounce
    openDoor();
    while (digitalRead(openButtonPin) == LOW); // wait for release
  }

  // Listen to serial from Python
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "OPEN" && !isLocked) {
      openDoor();
    }
  }
}

void updateStatus() {
  // Update RGB LED
  digitalWrite(redLEDPin, isLocked ? HIGH : LOW);
  digitalWrite(greenLEDPin, isLocked ? LOW : HIGH);
  digitalWrite(blueLEDPin, LOW); // Not used

  // Update LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Door is:");
  lcd.setCursor(0, 1);
  lcd.print(isLocked ? "LOCKED 🔒" : "UNLOCKED 🔓");
}

void openDoor() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Opening door...");
  doorServo.write(90);  // Open
  delay(doorclosetime);
  doorServo.write(0);   // Close
  updateStatus();
}
