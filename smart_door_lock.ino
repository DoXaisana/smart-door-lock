#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int lockButtonPin = 2;
const int openButtonPin = 3;
const int redLEDPin = 9;
const int greenLEDPin = 10;
const int servoPin = 11;

bool isLocked = true;
bool lastLockButtonState = LOW;
bool lastOpenButtonState = LOW;

Servo doorServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 columns, 2 rows

void setup() {
  pinMode(lockButtonPin, INPUT);
  pinMode(openButtonPin, INPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  doorServo.attach(servoPin);
  doorServo.write(0); // Initial closed position

  lcd.init();
  lcd.backlight();
  updateLED();
  updateLCD();
}

void loop() {
  bool lockButtonState = digitalRead(lockButtonPin);
  bool openButtonState = digitalRead(openButtonPin);

  // Handle lock/unlock status toggle
  if (lockButtonState == HIGH && lastLockButtonState == LOW) {
    isLocked = !isLocked;
    updateLED();
    updateLCD();
    delay(200);
  }
  lastLockButtonState = lockButtonState;

  // Handle door open
  if (openButtonState == HIGH && lastOpenButtonState == LOW) {
    if (!isLocked) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door Opening...");
      doorServo.write(90);  // Open
      delay(5000);          // Hold open for 2 seconds
      doorServo.write(0);   // Close
      lcd.clear();
      updateLCD();
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Can't Open Door");
      lcd.setCursor(0, 1);
      lcd.print("It's Locked");
      delay(1500);
      lcd.clear();
      updateLCD();
    }
    delay(200);
  }
  lastOpenButtonState = openButtonState;
}

// Update RGB LED status
void updateLED() {
  digitalWrite(redLEDPin, isLocked ? HIGH : LOW);
  digitalWrite(greenLEDPin, isLocked ? LOW : HIGH);
}

// Update LCD screen
void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Status: ");
  lcd.print(isLocked ? "LOCKED   " : "UNLOCKED ");
  lcd.setCursor(0, 1);
  lcd.print("Press Open Btn");
}