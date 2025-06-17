# 🔐 Smart Door Lock System

This project is a smart door lock system built using **Arduino Uno**, **Flask (Python)**, and **PostgreSQL** for logging. It allows users to:
- Lock/unlock the door via push buttons.
- View the current status on an LCD and RGB LED.
- Control door opening using a servo motor.
- View event logs on a Flask-powered website.
  
## 🧰 Hardware Components

| Component        | Quantity |
|------------------|----------|
| Arduino Uno      | 1        |
| Push Button      | 2        | (1 for Lock/Unlock status, 1 for Door Open)
| RGB LED          | 1        |
| Servo Motor      | 1        | (acts as the door)
| I2C LCD (16x2 or 20x4) | 1 |
| Resistors (10kΩ) | 2        |
| Breadboard + Jumper Wires | As needed |
| Power Supply     | USB or 5V Adapter |

---

## 🔌 Hardware Pin Connections

| Function         | Arduino Pin | Notes                             |
|------------------|-------------|-----------------------------------|
| Lock/Unlock Button | D2          | Pull-down resistor (10kΩ) needed |
| Open Door Button | D3          | Pull-down resistor (10kΩ) needed |
| RGB LED Red Pin  | D5          | Use current-limiting resistor     |
| RGB LED Green Pin| D6          | Use current-limiting resistor     |
| RGB LED Blue Pin | D7          | Optional                          |
| Servo Motor      | D9          | PWM-capable pin                  |
| I2C LCD SDA      | A4          | I2C Communication                 |
| I2C LCD SCL      | A5          | I2C Communication                 |
| GND              | GND         | Common ground for all components |
| 5V               | 5V          | Power supply for servo, buttons  |

> ⚠️ **Important:** Use a separate power source or capacitor for the servo if it causes resets.

---

## 💻 Software Requirements

### On Arduino (via Arduino IDE / PlatformIO)
- `Servo.h`
- `Wire.h`
- `LiquidCrystal_I2C.h`

### On Python/Flask (PC/Raspberry Pi)
- Python 3.9+
- Flask
- Flask SQLAlchemy
- psycopg2-binary
- Serial communication via `pyserial`

Install required packages:
```bash
pip install flask flask_sqlalchemy psycopg2-binary pyserial
