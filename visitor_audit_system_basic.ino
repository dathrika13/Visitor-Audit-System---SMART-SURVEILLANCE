/*
 * VISITOR AUDIT SYSTEM - BASIC VERSION (Without LoRa)
 * 
 * Description: Simplified visitor counting system for testing
 * Use this version if you don't have LoRa module or want to test basic functionality
 * 
 * Hardware Required:
 * - Arduino Nano (ATmega328P)
 * - 2x HC-SR04 Ultrasonic Sensors
 * - Buzzer (optional)
 * 
 * Version: 1.0 (Basic)
 */

// ============================
// PIN DEFINITIONS
// ============================
#define TRIG_PIN_1    3    // Sensor 1 trigger (Entry)
#define ECHO_PIN_1    4    // Sensor 1 echo
#define TRIG_PIN_2    5    // Sensor 2 trigger (Exit)
#define ECHO_PIN_2    6    // Sensor 2 echo
#define BUZZER_PIN    8    // Buzzer
#define BUTTON_PIN    7    // Reset button

// ============================
// CONFIGURATION
// ============================
#define DETECTION_DISTANCE  10    // Detection threshold in cm
#define BUZZER_DURATION     200   // Buzzer beep duration
#define DEBOUNCE_DELAY      500   // Anti-bounce delay

// ============================
// GLOBAL VARIABLES
// ============================
int visitorCount = 0;
int currentState1 = 0;
int previousState1 = 0;
int currentState2 = 0;
int previousState2 = 0;

// ============================
// SETUP
// ============================
void setup() {
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(TRIG_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIG_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  Serial.println("Visitor Audit System - Basic Version");
  Serial.println("=====================================");
  Serial.println("System Ready!");
  Serial.println("Visitor Count: 0");
  
  // Startup beep
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
}

// ============================
// MAIN LOOP
// ============================
void loop() {
  // Reset counter if button pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    visitorCount = 0;
    Serial.println("\n*** COUNTER RESET ***");
    Serial.print("Visitor Count: ");
    Serial.println(visitorCount);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
  }
  
  // Measure distances
  long distance1 = measureDistance(TRIG_PIN_1, ECHO_PIN_1);
  long distance2 = measureDistance(TRIG_PIN_2, ECHO_PIN_2);
  
  // ENTRY DETECTION (Sensor 1 → Sensor 2)
  if (distance1 <= DETECTION_DISTANCE && distance1 > 0) {
    currentState1 = 1;
  } else {
    currentState1 = 0;
  }
  
  if (currentState1 == 1 && previousState1 == 0) {
    // Wait for Sensor 2
    unsigned long startTime = millis();
    while (millis() - startTime < 2000) {
      long dist2 = measureDistance(TRIG_PIN_2, ECHO_PIN_2);
      if (dist2 <= DETECTION_DISTANCE && dist2 > 0) {
        visitorCount++;
        Serial.println("\n>>> ENTRY DETECTED <<<");
        Serial.print("Visitor Count: ");
        Serial.println(visitorCount);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(BUZZER_DURATION);
        digitalWrite(BUZZER_PIN, LOW);
        delay(DEBOUNCE_DELAY);
        break;
      }
      delay(50);
    }
  }
  previousState1 = currentState1;
  
  // EXIT DETECTION (Sensor 2 → Sensor 1)
  if (distance2 <= DETECTION_DISTANCE && distance2 > 0) {
    currentState2 = 1;
  } else {
    currentState2 = 0;
  }
  
  if (currentState2 == 1 && previousState2 == 0) {
    // Wait for Sensor 1
    unsigned long startTime = millis();
    while (millis() - startTime < 2000) {
      long dist1 = measureDistance(TRIG_PIN_1, ECHO_PIN_1);
      if (dist1 <= DETECTION_DISTANCE && dist1 > 0) {
        if (visitorCount > 0) visitorCount--;
        Serial.println("\n<<< EXIT DETECTED >>>");
        Serial.print("Visitor Count: ");
        Serial.println(visitorCount);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(BUZZER_DURATION);
        digitalWrite(BUZZER_PIN, LOW);
        delay(DEBOUNCE_DELAY);
        break;
      }
      delay(50);
    }
  }
  previousState2 = currentState2;
  
  delay(50);
}

// ============================
// DISTANCE MEASUREMENT
// ============================
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000);
  long distance = (duration / 2) / 29.1;
  
  if (distance >= 400 || distance <= 0) return 0;
  return distance;
}
