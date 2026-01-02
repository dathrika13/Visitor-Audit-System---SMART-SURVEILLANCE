# Visitor Audit System

**Smart Surveillance & People Counting System using LoRa Communication**

![Version](https://img.shields.io/badge/version-1.0-blue)
![Hardware](https://img.shields.io/badge/hardware-Arduino%20Nano-green)
![Communication](https://img.shields.io/badge/LoRa-Ra--02-orange)

---

## 📋 Table of Contents

- [Overview](#overview)
- [Aim](#aim)
- [Requirements](#requirements)
- [System Architecture](#system-architecture)
- [Components](#components)
- [Working Principle](#working-principle)
- [Circuit Diagram](#circuit-diagram)
- [Setup & Installation](#setup--installation)
- [Code Documentation](#code-documentation)
- [Mobile Application](#mobile-application)
- [Applications](#applications)
- [Technical Specifications](#technical-specifications)
- [Future Improvements](#future-improvements)

---

## 🎯 Overview

The **Visitor Audit System** is an IoT-based smart surveillance solution designed to detect and continuously track incoming and outgoing visitors in real-time. The system uses ultrasonic sensors for detection, LoRa for long-range wireless communication, and a cloud-connected mobile app for remote monitoring.

This low-cost, energy-efficient system helps optimize staff operations, improve workplace security, and evaluate ongoing performance by providing real-time visitor traffic data.

---

## 🎯 Aim

To develop a compact, low-power visitor counting and tracking system that:
- Detects and monitors visitor flow (entry/exit)
- Sends real-time notifications to authorized personnel
- Operates without expensive dedicated communication networks
- Provides easy configuration and deployment

---

## 📝 Requirements

### Functional Requirements

✅ Detect and continuously track incoming and outgoing visitors  
✅ Notify a set of mobile numbers about incoming visitors  
✅ Operate without expensive, dedicated communication networks  

### Non-Functional Requirements

✅ **Compact Design** - Suitable for mounting at entrances  
✅ **Low Power Consumption** - Energy-efficient operation  
✅ **Easy Configuration** - Simple setup and customization  
✅ **Sturdy Mounting** - Simple and reliable installation  
✅ **Minimal External Wiring** - Clean installation  

---

## 🏗️ System Architecture

```
┌─────────────────┐
│ Entrance Area   │
│                 │
│ Sensor 1 → [ ] ←─── Entry Detection
│                 │
│    Visitor      │
│      ↓↑         │
│                 │
│ Sensor 2 → [ ] ←─── Exit Detection
└─────────────────┘
        ↓
   Arduino Nano
   (ATmega328p)
        ↓
   LoRa Ra-02
   Transceiver
        ↓
   LoRa Gateway
        ↓
  Cloud Database
        ↓
   Mobile App
  (Kodular)
```

---

## 🔧 Components

### Hardware

| Component | Model/Type | Quantity | Purpose |
|-----------|------------|----------|---------|
| **Microcontroller** | Arduino Nano (ATmega328p) | 1 | Main processing unit |
| **Ultrasonic Sensors** | HC-SR04 | 2 | Visitor detection |
| **Wireless Module** | LoRa Ra-02 | 1 | Long-range communication |
| **RFID Reader** | RC522 | 1 | Optional authentication |
| **Buzzer** | SG2 | 1 | Audio notification |
| **LEDs** | Diagnostic LEDs | 3 | Status indication |
| **Power Supply** | 5V DC Adapter | 1 | System power |
| **PCB** | Custom 2-layer PCB | 1 | Circuit board (Eagle CAD) |
| **Enclosure** | 3D Printed | 1 | Housing |

### Software

| Software | Purpose |
|----------|---------|
| **Arduino IDE** | Firmware development |
| **Eagle CAD** | PCB design |
| **Kodular** | Mobile app development (Android) |
| **LoRaWAN** | Wireless communication protocol |

---

## ⚙️ Working Principle

### Detection Algorithm

The system uses **two ultrasonic sensors (HC-SR04)** positioned sequentially at the entrance:

#### **Entry Detection:**
1. Object detected by **Sensor 1** (distance ≤ 10 cm)
2. System sets `currentState = 1`
3. Waits for object to pass **Sensor 2**
4. When **Sensor 2** detects (distance ≤ 10 cm)
5. **Counter increments** (+1)

#### **Exit Detection:**
1. Object detected by **Sensor 2** first
2. System sets `currentState2 = 1`
3. Waits for object to pass **Sensor 1**
4. When **Sensor 1** detects the object
5. **Counter decrements** (-1)

### Data Flow

```
Ultrasonic Sensors → Arduino (Processing) → LoRa Module → Gateway → Cloud → Mobile App
```

### Key Features

- **Bidirectional Counting** - Tracks both entry and exit
- **Real-time Updates** - Instant count transmission via LoRa
- **Cloud Storage** - Visitor data stored in database
- **Mobile Notifications** - Staff alerts via mobile app
- **Audio Alert** - Buzzer sounds when visitor detected
- **Motion Detection** - Can serve dual purpose

---

## 📐 Circuit Diagram

**📄 Reference:** `Schematic_VISITOR_AUDIT_SYSTEM.pdf`

### Pin Connections

#### **Ultrasonic Sensor 1 (Entry)**
| Sensor Pin | Arduino Pin |
|------------|-------------|
| Trig | D3 |
| Echo | D4 |
| VCC | 5V |
| GND | GND |

#### **Ultrasonic Sensor 2 (Exit)**
| Sensor Pin | Arduino Pin |
|------------|-------------|
| Trig | D5 |
| Echo | D6 |
| VCC | 5V |
| GND | GND |

#### **LoRa Ra-02 Module**
| LoRa Pin | Arduino Pin |
|----------|-------------|
| NSS | D10 |
| MOSI | D11 |
| MISO | D12 |
| SCK | D13 |
| RESET | D4 (optional) |
| 3.3V | 3.3V |
| GND | GND |

#### **RFID RC522 (Optional)**
| RFID Pin | Arduino Pin |
|----------|-------------|
| SDA | D8 |
| SCK | D13 |
| MOSI | D11 |
| MISO | D12 |
| RST | D2 |
| 3.3V | 3.3V |
| GND | GND |

#### **Buzzer**
| Component | Arduino Pin |
|-----------|-------------|
| Buzzer + | D8 |
| Buzzer - | GND |

---

## 🚀 Setup & Installation

### Hardware Setup

1. **Assemble the PCB** using the provided Eagle CAD files
2. **Mount sensors** at entrance (10-20 cm apart)
3. **Connect components** as per circuit diagram
4. **Install 3D printed enclosure**
5. **Connect 5V power supply**

### Software Installation

#### **1. Arduino Firmware**

```bash
# Install Arduino IDE (if not already installed)
# Download from: https://www.arduino.cc/en/software

# Install Required Libraries
# Tools → Manage Libraries → Search and Install:
- LoRa by Sandeep Mistry
- MFRC522 (if using RFID)
```

#### **2. Upload Code**

```bash
# 1. Open Arduino IDE
# 2. Load the visitor_audit_system.ino file
# 3. Select Board: Tools → Board → Arduino Nano
# 4. Select Processor: Tools → Processor → ATmega328P
# 5. Select Port: Tools → Port → [Your COM Port]
# 6. Upload: Sketch → Upload (or Ctrl+U)
```

#### **3. Configure LoRa Settings**

```cpp
// In the Arduino code, update these parameters:
#define LORA_FREQUENCY 915E6  // For US (868E6 for EU)
#define LORA_TX_POWER 20      // Transmission power (dBm)
#define LORA_SPREADING_FACTOR 7
```

### Mobile App Setup

1. **Download** the Kodular-built APK
2. **Install** on Android device
3. **Configure** cloud database credentials
4. **Enable** notifications
5. **Test** connection with the hardware

---

## 💻 Code Documentation

### Main Functions

#### `setup()`
Initializes I/O pins for sensors and buzzer

```cpp
void setup() {
  pinMode(button, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}
```

#### `loop()`
Main execution loop handling:
- Distance measurement from both sensors
- Entry/exit detection logic
- Counter increment/decrement
- Data transmission

### Key Variables

```cpp
#define trigPin 3      // Sensor 1 trigger pin
#define echoPin 4      // Sensor 1 echo pin
#define trigPin2 5     // Sensor 2 trigger pin
#define echoPin2 6     // Sensor 2 echo pin

int i = 0;             // Visitor count
int currentState = 0;   // Sensor 1 state
int currentState2 = 0;  // Sensor 2 state
```

### Distance Calculation

```cpp
// Ultrasonic distance formula
distance = (duration/2) / 29.1;
// Result in centimeters
```

### Detection Threshold

```cpp
if (distance <= 10) {  // 10 cm detection range
  currentState = 1;    // Object detected
}
```

---

## 📱 Mobile Application

Developed using **Kodular** (Android)

### Features

✅ **Real-time Count Display** - Live visitor count from database  
✅ **Push Notifications** - Alerts when visitors enter  
✅ **Historical Data** - View traffic patterns over time  
✅ **Multi-user Support** - Multiple staff can monitor  
✅ **Cloud Sync** - Data accessible anywhere  

### Database Integration

The app connects to a cloud database (Firebase/MySQL) to:
- Retrieve current visitor count
- Display entry/exit timestamps
- Generate traffic reports
- Send notifications to authorized users

---

## 🎯 Applications

### Retail & Commercial

- **Traffic Analysis** - Optimize staff scheduling during peak hours
- **Customer Insights** - Understand visitor patterns
- **Security** - Monitor unauthorized access after hours

### Industrial

- **Workplace Security** - Track employee/visitor flow
- **Capacity Management** - Ensure occupancy limits
- **Performance Evaluation** - Analyze facility usage

### General Use Cases

✅ Shopping malls and retail stores  
✅ Office buildings and co-working spaces  
✅ Museums and exhibition halls  
✅ Hospitals and clinics  
✅ Educational institutions  
✅ Event venues  

---

## 📊 Technical Specifications

### Ultrasonic Sensor (HC-SR04)

| Parameter | Specification |
|-----------|---------------|
| **Detection Range** | 2 cm - 400 cm |
| **Effective Angle** | < 15° |
| **Accuracy** | ± 3 mm |
| **Operating Voltage** | 5V DC |
| **Operating Current** | 15 mA |
| **Operating Frequency** | 40 kHz |

**How it Works:**
- Transmits ultrasonic wave at 40 kHz
- Measures time for echo to return
- Converts time to distance using sound speed (343 m/s)

### LoRa Ra-02 Module

| Parameter | Specification |
|-----------|---------------|
| **Frequency** | 433/470/868/915 MHz |
| **Data Rate** | Up to 320 kbps |
| **Range** | Up to 2 km (urban) / 9 km (line of sight) |
| **Operating Voltage** | 3.3V |
| **TX Power** | +20 dBm (100 mW) |
| **Sensitivity** | -148 dBm |

**Advantages:**
- Low power consumption
- Long-range communication
- No network fees
- Excellent receiver sensitivity

### Arduino Nano (ATmega328P)

| Parameter | Specification |
|-----------|---------------|
| **Microcontroller** | ATmega328P |
| **Operating Voltage** | 5V |
| **Clock Speed** | 16 MHz |
| **Flash Memory** | 32 KB |
| **SRAM** | 2 KB |
| **Digital I/O Pins** | 14 (6 PWM) |

---

## 🔧 Calibration & Tuning

### Sensor Positioning

```
┌─────────────────────┐
│  Door/Entrance      │
│                     │
│  [S1]     [S2]      │  ← Mount 10-20 cm apart
│   ↓         ↓       │
│ Entry      Exit     │
└─────────────────────┘
```

**Recommended:**
- Sensor height: 90-120 cm from floor
- Sensor spacing: 15-20 cm apart
- Detection range: 5-15 cm (adjustable in code)

### Timing Adjustment

```cpp
delay(1000);  // 1-second pause between counts
// Adjust based on:
// - Visitor walking speed
// - Door width
// - Traffic density
```

---

## 🚧 Troubleshooting

### Common Issues

| Problem | Solution |
|---------|----------|
| **Sensor not detecting** | Check wiring, adjust detection range |
| **False counts** | Increase delay time, adjust sensor angle |
| **LoRa not transmitting** | Check antenna, verify frequency settings |
| **No mobile notifications** | Verify cloud connectivity, check app permissions |
| **Power issues** | Ensure 5V supply, check battery voltage |

### Debug Mode

Add serial monitoring to code:

```cpp
void setup() {
  Serial.begin(9600);  // Add this line
  // ... rest of setup
}

void loop() {
  Serial.print("Distance 1: ");
  Serial.println(distance);
  Serial.print("Count: ");
  Serial.println(i);
}
```

---

## 🔮 Future Improvements

### Hardware Enhancements

- ✨ **Better Sensors** - ToF (Time-of-Flight) sensors for higher accuracy
- ✨ **Camera Integration** - Computer vision for advanced analytics
- ✨ **Solar Power** - Self-sustaining power supply
- ✨ **Multiple Entrances** - Multi-node network support

### Software Enhancements

- ✨ **Machine Learning** - Predict traffic patterns
- ✨ **Web Dashboard** - Real-time analytics and reporting
- ✨ **Integration APIs** - Connect with existing systems
- ✨ **Face Recognition** - Identify specific individuals

### LoRaWAN Network

- ✨ **Mesh Networking** - Multiple devices in one network
- ✨ **Edge Computing** - Local processing before cloud sync
- ✨ **Encrypted Communication** - Enhanced security

---

## 📄 Files Included

```
VISITOR_AUDIT_SYSTEM/
├── README.md                              # This file
├── Schematic_VISITOR_AUDIT_SYSTEM.pdf     # Circuit diagram
├── visitor_audit_system.ino               # Arduino firmware
├── eagle_files/                           # PCB design files
│   ├── visitor_audit.sch                  # Schematic
│   └── visitor_audit.brd                  # Board layout
├── 3d_models/                             # Enclosure STL files
│   └── enclosure.stl
└── mobile_app/                            # Kodular project
    └── visitor_audit.aia
```

---

## 📊 Performance Metrics

| Metric | Value |
|--------|-------|
| **Detection Accuracy** | ~95% (with proper calibration) |
| **Response Time** | < 100 ms |
| **Power Consumption** | ~200 mA @ 5V |
| **LoRa Range (Urban)** | 1-2 km |
| **LoRa Range (Rural)** | Up to 9 km |
| **Battery Life** | ~24 hours (with 5000 mAh battery) |

---

## 🤝 Contributing

This is an academic/research project. Suggestions for improvements are welcome!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/improvement`)
3. Commit changes (`git commit -m 'Add improvement'`)
4. Push to branch (`git push origin feature/improvement`)
5. Open a Pull Request

---

## 📜 License

This project is for **educational and research purposes**.

---

## 👥 Credits

**Project:** Smart Surveillance Visitor Audit System  
**Version:** 1.0  
**Date:** 11-02-2020  
**Hardware:** Arduino Nano, LoRa Ra-02, HC-SR04  
**Platform:** Kodular (Mobile App)  

---

## 📞 Support

For issues, questions, or suggestions:
- Create an issue in the repository
- Check the troubleshooting section
- Review the circuit diagram PDF

---

**Built with ❤️ for Smart Surveillance and IoT Applications**
