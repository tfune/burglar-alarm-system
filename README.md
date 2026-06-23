# Raspberry Pi Burglar Alarm System

An embedded security monitoring system built on a Raspberry Pi 4 using motion sensing, ADC-based temperature monitoring, keypad-controlled arming/disarming, and LCD-based system status feedback.

This project was developed to demonstrate core embedded systems concepts including:

* GPIO interfacing
* I²C peripheral communication
* ADC-based sensor monitoring
* embedded user interfaces
* alarm state management
* hardware/software integration
* embedded debugging and reliability improvements

## Features

* Keypad-controlled arm/disarm operation
* PIR motion sensor monitoring
* Thermistor-based temperature change detection
* ADS7830 I²C ADC integration
* 16x2 I²C LCD user interface
* Independent motion and temperature alarms
* Configurable alarm thresholds
* LCD startup reliability improvements
* Makefile-based build system
* Modular project organization

## Demo

### YouTube Demonstration

*Add YouTube link here*

Watch the full project demonstration showing system arming, motion detection, temperature alarm activation, and LCD status updates.

---

## System Overview

### System Overview

*Add system overview image*

### Armed State

*Add LCD armed image*

### Disarmed State

*Add LCD disarmed image*

### System Architecture Diagram

![System Architecture](docs/block-diagram.png)

---

## Hardware Used

| Component          | Description                        |
| ------------------ | ---------------------------------- |
| Controller         | Raspberry Pi 4 Model B             |
| Motion Sensor      | PIR Motion Sensor                  |
| ADC                | ADS7830 I²C ADC                    |
| Temperature Sensor | Thermistor Voltage Divider         |
| Display            | 16x2 I²C LCD with PCF8574 Backpack |
| Input              | 4x4 Matrix Keypad                  |
| Alarm Output       | Active Buzzer                      |
| Visual Indicator   | LED                                |

---

## System Architecture

```text
BurglarAlarmSystem
├── Keypad Input
├── PIR Motion Sensor
├── ADS7830 ADC
├── Thermistor Sensor
├── LCD Interface
├── Buzzer Alarm
└── LED Alarm
```

---

## Subsystem Responsibilities

### Keypad Interface

* arm/disarm command input
* keypad scanning and debouncing
* user interaction handling

### Motion Detection

* PIR sensor monitoring
* motion alarm generation
* buzzer activation control

### Temperature Monitoring

* periodic ADC sampling
* baseline value establishment
* temperature change detection
* alarm threshold comparison

### LCD Interface

* system status display
* armed/disarmed indication
* user feedback

### Alarm Outputs

* buzzer control
* LED control
* independent alarm operation

---

## System Operation

### DISARMED

* motion alarms disabled
* temperature alarms disabled
* buzzer off
* LED off

### ARMED

* PIR sensor actively monitored
* thermistor readings periodically sampled
* motion detection activates buzzer
* temperature changes activate LED

### Controls

| Key | Function      |
| --- | ------------- |
| A   | Arm System    |
| D   | Disarm System |

---

## Temperature Detection Method

The thermistor voltage divider is sampled through the ADS7830 ADC.

At startup, the system records a baseline ADC value.

Subsequent readings are compared against this baseline:

```cpp
adcChange = abs(adcValue - baselineADC);
```

If the change exceeds the configured threshold:

```cpp
tempAlarm = (adcChange >= ADC_CHANGE_THRESHOLD);
```

the temperature alarm becomes active.

This approach allows the system to detect meaningful environmental changes rather than relying on a fixed temperature threshold.

---

## Reliability Improvements

### LCD Initialization Recovery

During testing, the LCD occasionally powered up displaying only white boxes despite remaining visible on the I²C bus at address `0x27`.

Troubleshooting included:

* I²C bus verification using `i2cdetect`
* LCD-only test applications
* isolation of application software from hardware initialization

The issue appeared to be related to startup timing or LCD initialization reliability rather than application logic.

To improve robustness, LCD initialization retry logic was implemented:

```cpp
for (int attempt = 0; attempt < LCD_INIT_ATTEMPTS; attempt++)
```

This allows the system to automatically recover from initialization failures during startup.

---

## Embedded Concepts Demonstrated

* GPIO interfacing
* I²C communication
* ADC-based sensor monitoring
* Embedded user interfaces
* Keypad scanning and debouncing
* State-based alarm control
* Hardware initialization
* Reliability improvements
* Build automation with Makefiles
* Hardware/software integration
* Embedded debugging and troubleshooting

---

## Build and Run

Build the project:

```bash
make
```

Run the project:

```bash
sudo ./burglar_alarm
```

Build and run:

```bash
make run
```

Clean build files:

```bash
make clean
```

---

## Future Improvements

* PIN-protected arming and disarming
* Alarm event logging
* Additional fault diagnostics
* Audible alarm patterns for different fault types

---

## Author

Trevor Fune
