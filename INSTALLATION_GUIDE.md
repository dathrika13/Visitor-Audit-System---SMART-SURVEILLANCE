# Arduino Code Installation Guide
## Visitor Audit System

---

## 📦 Files Included

- `visitor_audit_system.ino` - Main code with LoRa support
- `visitor_audit_system_basic.ino` - Simplified version without LoRa
- `config.h` - Configuration file (optional)

---

## 🔧 Prerequisites

### Software
1. **Arduino IDE** (version 1.8.x or 2.x)
   - Download: https://www.arduino.cc/en/software

### Libraries Required

#### For Full Version (with LoRa):
1. **LoRa by Sandeep Mistry**
   - Install via: Tools → Manage Libraries → Search "LoRa" → Install

#### For Basic Version:
- No additional libraries needed!

---

## 📝 Installation Steps

### Step 1: Install Arduino IDE

```bash
# Download and install from:
https://www.arduino.cc/en/software

# For Linux:
sudo apt-get update
sudo apt-get install arduino

# For macOS (using Homebrew):
brew install --cask arduino
```

### Step 2: Install Required Libraries

**Method 1: Using Library Manager (Recommended)**

1. Open Arduino IDE
2. Go to **Tools → Manage Libraries...**
3. Search for "**LoRa**"
4. Install "**LoRa by Sandeep Mistry**"
5. Click "Install" and wait for completion

**Method 2: Manual Installation**

1. Download library from: https://github.com/sandeepmistry/arduino-LoRa
2. Extract to Arduino libraries folder:
   - Windows: `Documents\Arduino\libraries\`
   - Mac: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`
3. Restart Arduino IDE

### Step 3: Select Board and Port

1. Connect Arduino Nano to computer via USB
2. In Arduino IDE:
   - **Tools → Board → Arduino AVR Boards → Arduino Nano**
   - **Tools → Processor → ATmega328P** (or ATmega328P Old Bootloader)
   - **Tools → Port → [Select your COM port]**

**Note:** If you don't see your port, install CH340 drivers:
- Download: http://www.wch-ic.com/downloads/CH341SER_ZIP.html

### Step 4: Open the Code

1. **For Full Version (with LoRa):**
   - File → Open → `visitor_audit_system.ino`

2. **For Basic Version (testing only):**
   - File → Open → `visitor_audit_system_basic.ino`

### Step 5: Configure Settings (Optional)

Open `config.h` and modify:

```cpp
// Change detection distance (cm)
#define DETECTION_DISTANCE    10

// Change LoRa frequency based on region
#define LORA_FREQUENCY        915E6  // 915 MHz for US
// #define LORA_FREQUENCY     868E6  // 868 MHz for EU
// #define LORA_FREQUENCY     433E6  // 433 MHz for Asia

// Change device ID
#define DEVICE_ID             "VAS-001"
```

### Step 6: Verify Code

1. Click the **✓ Verify** button (or Ctrl+R)
2. Wait for compilation to complete
3. Check for errors in the console

### Step 7: Upload to Arduino

1. Click the **→ Upload** button (or Ctrl+U)
2. Wait for "Done uploading" message
3. If upload fails:
   - Try "ATmega328P (Old Bootloader)" processor
   - Check USB cable connection
   - Verify correct COM port selected

---

## 🔍 Testing & Debugging

### Open Serial Monitor

1. **Tools → Serial Monitor** (or Ctrl+Shift+M)
2. Set baud rate to **9600**
3. You should see:

```
Visitor Audit System Initializing...
Initializing LoRa...
LoRa initialized successfully!
System Ready!
================================
Visitor Count: 0
================================
```

### Test Sensors

1. Move your hand in front of **Sensor 1** (Entry)
2. Serial Monitor should show:
   ```
   Sensor 1 triggered - Waiting for Sensor 2...
   ```
3. Move hand in front of **Sensor 2**
4. You should see:
   ```
   ================================
   ENTRY DETECTED!
   Visitor Count: 1
   ================================
   ```

### Test Exit Detection

1. Move hand in front of **Sensor 2** first
2. Then move in front of **Sensor 1**
3. Counter should decrement

---

## ⚙️ Calibration Guide

### Adjusting Detection Distance

If sensors are too sensitive or not sensitive enough:

```cpp
// In config.h or main code:
#define DETECTION_DISTANCE 10  // Try 5, 8, 10, 15, 20
```

**Recommendations:**
- Small doorway: 8-10 cm
- Large doorway: 15-20 cm
- High traffic: 5-8 cm (more sensitive)

### Adjusting Timeout

If counter misses fast-moving people:

```cpp
#define DETECTION_TIMEOUT 3000  // Try 2000, 3000, 5000
```

### Debounce Delay

If getting double counts:

```cpp
#define DEBOUNCE_DELAY 500  // Try 300, 500, 1000
```

---

## 🐛 Troubleshooting

### Problem: LoRa initialization failed

**Solutions:**
1. Check wiring connections (NSS, MOSI, MISO, SCK)
2. Verify 3.3V power supply to LoRa module
3. Try different LoRa frequency
4. Check antenna connection

### Problem: Sensors not detecting

**Solutions:**
1. Check wiring (Trig/Echo pins)
2. Verify 5V power supply
3. Increase detection distance: `#define DETECTION_DISTANCE 20`
4. Test individual sensors:

```cpp
void loop() {
  long dist1 = measureDistance(TRIG_PIN_1, ECHO_PIN_1);
  Serial.print("Sensor 1: ");
  Serial.println(dist1);
  delay(500);
}
```

### Problem: Double counting

**Solutions:**
1. Increase debounce delay: `#define DEBOUNCE_DELAY 1000`
2. Adjust sensor spacing (make them farther apart)
3. Lower detection distance

### Problem: Upload failed / Port not found

**Solutions:**
1. Install CH340/CH341 USB drivers
2. Try different USB cable
3. Press reset button on Arduino while uploading
4. Select "ATmega328P (Old Bootloader)"

### Problem: Code compiles but doesn't work

**Solutions:**
1. Open Serial Monitor to see debug messages
2. Check if all pins match your wiring
3. Verify all components have proper power
4. Test basic version first (without LoRa)

---

## 📊 Pin Connection Reference

### Quick Reference Table

| Component | Pin | Arduino Nano |
|-----------|-----|--------------|
| Sensor 1 Trig | OUT | D3 |
| Sensor 1 Echo | IN | D4 |
| Sensor 2 Trig | OUT | D5 |
| Sensor 2 Echo | IN | D6 |
| Buzzer | OUT | D8 |
| Button | IN | D7 (with pullup) |
| LoRa NSS | OUT | D10 |
| LoRa MOSI | OUT | D11 |
| LoRa MISO | IN | D12 |
| LoRa SCK | OUT | D13 |
| LoRa RST | OUT | D9 |
| LED Entry | OUT | D2 |
| LED Exit | OUT | A0 |
| LED Status | OUT | A1 |

---

## 🎯 Usage Tips

### For Best Results:

1. **Sensor Placement:**
   - Mount 90-120 cm from floor
   - Space 15-20 cm apart
   - Point perpendicular to doorway

2. **Power Supply:**
   - Use regulated 5V adapter (at least 500mA)
   - Avoid USB power for final deployment
   - Add capacitors for stability

3. **Detection Range:**
   - Start with 10 cm
   - Adjust based on door width
   - Test with different walking speeds

4. **LoRa Range:**
   - Line of sight: up to 2 km
   - Indoor: 200-500 m
   - Use external antenna for better range

---

## 📱 Serial Monitor Commands

While serial monitor is open, you can:

- **View real-time count updates**
- **See sensor trigger events**
- **Monitor LoRa transmissions**
- **Debug distance readings**

Example output:
```
Sensor 1 triggered - Waiting for Sensor 2...
================================
ENTRY DETECTED!
Visitor Count: 1
================================
Sending LoRa packet: {"device":"VAS-001","count":1,"timestamp":15}
Packet sent successfully!
```

---

## 🚀 Next Steps

After successful testing:

1. **Deploy Hardware:**
   - Mount sensors at entrance
   - Install enclosure
   - Connect power supply

2. **Configure LoRa Gateway:**
   - Set up receiver to forward data to cloud
   - Configure database connection

3. **Setup Mobile App:**
   - Install Kodular app
   - Connect to cloud database
   - Enable notifications

4. **Fine-tune System:**
   - Adjust detection parameters
   - Optimize transmission interval
   - Test under real conditions

---

## 📞 Support

If you encounter issues:

1. Check troubleshooting section above
2. Verify all wiring connections
3. Test with basic version first
4. Check Serial Monitor for error messages
5. Review circuit diagram PDF

---

## 📄 Version History

- **v1.0** - Initial release with LoRa support
- **v1.0-basic** - Simplified version for testing

---

**Happy Coding! 🎉**
