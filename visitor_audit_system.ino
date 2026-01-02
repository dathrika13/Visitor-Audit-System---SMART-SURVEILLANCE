/*
 * VISITOR AUDIT SYSTEM - Smart Surveillance
 * 
 * Description: IoT-based visitor counting system using ultrasonic sensors
 * and LoRa communication for real-time tracking of incoming/outgoing visitors
 * 
 * Hardware:
 * - Arduino Nano (ATmega328P)
 * - 2x HC-SR04 Ultrasonic Sensors
 * - LoRa Ra-02 Module (SX1278)
 * - Buzzer
 * - LEDs for status indication
 * 
 * Version: 1.0
 * Date: 11-02-2020
 * Updated: 2025
 */

// ============================
// LIBRARY INCLUDES
// ============================
#include <SPI.h>
#include <LoRa.h>

// ============================
// PIN DEFINITIONS
// ============================
// Ultrasonic Sensor 1 (Entry Detection)
#define TRIG_PIN_1    3
#define ECHO_PIN_1    4

// Ultrasonic Sensor 2 (Exit Detection)
#define TRIG_PIN_2    5
#define ECHO_PIN_2    6

// Buzzer
#define BUZZER_PIN    8

// Optional Button (Reset Counter)
#define BUTTON_PIN    7

// LoRa Module Pins (SPI Interface)
// NSS (Chip Select) = Pin 10
// MOSI = Pin 11
// MISO = Pin 12
// SCK = Pin 13
// RESET = Pin 9 (optional)
#define LORA_NSS      10
#define LORA_RESET    9

// Status LEDs (Optional)
#define LED_ENTRY     2   // Lights up on entry
#define LED_EXIT      A0  // Lights up on exit
#define LED_STATUS    A1  // LoRa transmission status

// ============================
// CONFIGURATION PARAMETERS
// ============================
#define DETECTION_DISTANCE  10    // Detection threshold in cm
#define BUZZER_DURATION     200   // Buzzer beep duration in ms
#define DEBOUNCE_DELAY      500   // Delay to prevent double counting
#define LORA_FREQUENCY      915E6 // 915 MHz for US (868E6 for EU)
#define LORA_TX_POWER       20    // Transmission power in dBm
#define LORA_SYNC_WORD      0x12  // Sync word for network

// ============================
// GLOBAL VARIABLES
// ============================
int visitorCount = 0;           // Current visitor count
int currentState1 = 0;          // Sensor 1 current state
int previousState1 = 0;         // Sensor 1 previous state
int currentState2 = 0;          // Sensor 2 current state
int previousState2 = 0;         // Sensor 2 previous state

unsigned long lastTransmission = 0;  // Last LoRa transmission time
unsigned long transmissionInterval = 5000;  // Send data every 5 seconds

// ============================
// FUNCTION PROTOTYPES
// ============================
long measureDistance(int trigPin, int echoPin);
void soundBuzzer(int duration);
void sendLoRaData();
void blinkLED(int pin, int times);
void checkEntryDetection();
void checkExitDetection();

// ============================
// SETUP FUNCTION
// ============================
void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Visitor Audit System Initializing...");
  
  // Initialize I/O Pins
  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_ENTRY, OUTPUT);
  pinMode(LED_EXIT, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  
  // Turn off all LEDs initially
  digitalWrite(LED_ENTRY, LOW);
  digitalWrite(LED_EXIT, LOW);
  digitalWrite(LED_STATUS, LOW);
  
  // Initialize LoRa Module
  Serial.println("Initializing LoRa...");
  LoRa.setPins(LORA_NSS, LORA_RESET, -1);
  
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("LoRa initialization failed!");
    // Blink status LED rapidly to indicate error
    while (1) {
      digitalWrite(LED_STATUS, !digitalRead(LED_STATUS));
      delay(100);
    }
  }
  
  // Configure LoRa Parameters
  LoRa.setTxPower(LORA_TX_POWER);
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setSyncWord(LORA_SYNC_WORD);
  
  Serial.println("LoRa initialized successfully!");
  
  // Startup indication
  soundBuzzer(100);
  delay(100);
  soundBuzzer(100);
  blinkLED(LED_STATUS, 3);
  
  Serial.println("System Ready!");
  Serial.println("================================");
  Serial.println("Visitor Count: 0");
  Serial.println("================================");
}

// ============================
// MAIN LOOP
// ============================
void loop() {
  // Check for manual reset button press
  if (digitalRead(BUTTON_PIN) == LOW) {
    visitorCount = 0;
    Serial.println("Counter Reset!");
    soundBuzzer(50);
    delay(500);
  }
  
  // Check for entry detection (Sensor 1 first, then Sensor 2)
  checkEntryDetection();
  
  // Check for exit detection (Sensor 2 first, then Sensor 1)
  checkExitDetection();
  
  // Send data via LoRa periodically
  if (millis() - lastTransmission >= transmissionInterval) {
    sendLoRaData();
    lastTransmission = millis();
  }
  
  // Small delay to stabilize readings
  delay(50);
}

// ============================
// ENTRY DETECTION FUNCTION
// ============================
void checkEntryDetection() {
  long distance1 = measureDistance(TRIG_PIN_1, ECHO_PIN_1);
  
  // Object detected by Sensor 1 (Entry side)
  if (distance1 <= DETECTION_DISTANCE && distance1 > 0) {
    currentState1 = 1;
  } else {
    currentState1 = 0;
  }
  
  // State change detected on Sensor 1
  if (currentState1 != previousState1) {
    if (currentState1 == 1) {
      // Object entered Sensor 1 range, wait for Sensor 2
      Serial.println("Sensor 1 triggered - Waiting for Sensor 2...");
      digitalWrite(LED_ENTRY, HIGH);
      soundBuzzer(50);
      
      unsigned long startTime = millis();
      boolean sensor2Triggered = false;
      
      // Wait up to 3 seconds for Sensor 2 detection
      while (millis() - startTime < 3000) {
        long distance2 = measureDistance(TRIG_PIN_2, ECHO_PIN_2);
        
        if (distance2 <= DETECTION_DISTANCE && distance2 > 0) {
          sensor2Triggered = true;
          break;
        }
        delay(50);
      }
      
      if (sensor2Triggered) {
        // Valid entry detected
        visitorCount++;
        Serial.println("================================");
        Serial.println("ENTRY DETECTED!");
        Serial.print("Visitor Count: ");
        Serial.println(visitorCount);
        Serial.println("================================");
        
        soundBuzzer(BUZZER_DURATION);
        sendLoRaData();  // Send immediately on entry
        delay(DEBOUNCE_DELAY);
      }
      
      digitalWrite(LED_ENTRY, LOW);
    }
    previousState1 = currentState1;
  }
}

// ============================
// EXIT DETECTION FUNCTION
// ============================
void checkExitDetection() {
  long distance2 = measureDistance(TRIG_PIN_2, ECHO_PIN_2);
  
  // Object detected by Sensor 2 (Exit side)
  if (distance2 <= DETECTION_DISTANCE && distance2 > 0) {
    currentState2 = 1;
  } else {
    currentState2 = 0;
  }
  
  // State change detected on Sensor 2
  if (currentState2 != previousState2) {
    if (currentState2 == 1) {
      // Object entered Sensor 2 range, wait for Sensor 1
      Serial.println("Sensor 2 triggered - Waiting for Sensor 1...");
      digitalWrite(LED_EXIT, HIGH);
      soundBuzzer(50);
      
      unsigned long startTime = millis();
      boolean sensor1Triggered = false;
      
      // Wait up to 3 seconds for Sensor 1 detection
      while (millis() - startTime < 3000) {
        long distance1 = measureDistance(TRIG_PIN_1, ECHO_PIN_1);
        
        if (distance1 <= DETECTION_DISTANCE && distance1 > 0) {
          sensor1Triggered = true;
          break;
        }
        delay(50);
      }
      
      if (sensor1Triggered) {
        // Valid exit detected
        if (visitorCount > 0) {
          visitorCount--;
        }
        Serial.println("================================");
        Serial.println("EXIT DETECTED!");
        Serial.print("Visitor Count: ");
        Serial.println(visitorCount);
        Serial.println("================================");
        
        soundBuzzer(BUZZER_DURATION);
        sendLoRaData();  // Send immediately on exit
        delay(DEBOUNCE_DELAY);
      }
      
      digitalWrite(LED_EXIT, LOW);
    }
    previousState2 = currentState2;
  }
}

// ============================
// DISTANCE MEASUREMENT FUNCTION
// ============================
long measureDistance(int trigPin, int echoPin) {
  long duration, distance;
  
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure echo time
  duration = pulseIn(echoPin, HIGH, 30000);  // Timeout after 30ms
  
  // Calculate distance in cm
  // Speed of sound = 343 m/s = 29.1 microseconds per cm
  distance = (duration / 2) / 29.1;
  
  // Return 0 if out of range or error
  if (distance >= 400 || distance <= 0) {
    return 0;
  }
  
  return distance;
}

// ============================
// BUZZER CONTROL FUNCTION
// ============================
void soundBuzzer(int duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

// ============================
// LED BLINK FUNCTION
// ============================
void blinkLED(int pin, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(200);
    digitalWrite(pin, LOW);
    delay(200);
  }
}

// ============================
// LORA DATA TRANSMISSION
// ============================
void sendLoRaData() {
  digitalWrite(LED_STATUS, HIGH);
  
  // Create JSON-formatted data packet
  String dataPacket = "{";
  dataPacket += "\"device\":\"VAS-001\",";
  dataPacket += "\"count\":";
  dataPacket += String(visitorCount);
  dataPacket += ",\"timestamp\":";
  dataPacket += String(millis() / 1000);  // Time in seconds
  dataPacket += "}";
  
  // Send packet via LoRa
  Serial.print("Sending LoRa packet: ");
  Serial.println(dataPacket);
  
  LoRa.beginPacket();
  LoRa.print(dataPacket);
  LoRa.endPacket();
  
  Serial.println("Packet sent successfully!");
  
  digitalWrite(LED_STATUS, LOW);
}

// ============================
// OPTIONAL: RECEIVE LORA DATA
// ============================
// Uncomment to enable receiving commands from gateway
/*
void receiveLoRaData() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    Serial.print("Received packet: ");
    
    String received = "";
    while (LoRa.available()) {
      received += (char)LoRa.read();
    }
    
    Serial.println(received);
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());
    
    // Parse commands (e.g., "RESET" to reset counter)
    if (received == "RESET") {
      visitorCount = 0;
      Serial.println("Counter reset via LoRa command");
      soundBuzzer(100);
    }
  }
}
*/
