/*
 * CONFIG.H - Configuration File for Visitor Audit System
 * 
 * Modify these settings according to your hardware setup and requirements
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================
// HARDWARE PIN CONFIGURATION
// ============================

// Ultrasonic Sensor 1 (Entry Detection)
#define TRIG_PIN_1    3
#define ECHO_PIN_1    4

// Ultrasonic Sensor 2 (Exit Detection)
#define TRIG_PIN_2    5
#define ECHO_PIN_2    6

// Buzzer Pin
#define BUZZER_PIN    8

// Button Pin (Reset Counter)
#define BUTTON_PIN    7

// LoRa Module Pins
#define LORA_NSS      10    // Chip Select
#define LORA_RESET    9     // Reset Pin

// Optional Status LEDs
#define LED_ENTRY     2     // Entry indicator
#define LED_EXIT      A0    // Exit indicator
#define LED_STATUS    A1    // LoRa status

// ============================
// DETECTION PARAMETERS
// ============================

// Distance threshold for object detection (in centimeters)
// Objects closer than this distance will trigger the sensor
#define DETECTION_DISTANCE    10

// Maximum time to wait for second sensor (in milliseconds)
// If second sensor doesn't trigger within this time, event is ignored
#define DETECTION_TIMEOUT     3000

// Debounce delay to prevent double counting (in milliseconds)
#define DEBOUNCE_DELAY        500

// ============================
// BUZZER SETTINGS
// ============================

// Enable/disable buzzer (true = ON, false = OFF)
#define BUZZER_ENABLED        true

// Buzzer beep duration (in milliseconds)
#define BUZZER_DURATION       200

// ============================
// LORA CONFIGURATION
// ============================

// LoRa Frequency
// 915E6 for US, 868E6 for Europe, 433E6 for Asia
#define LORA_FREQUENCY        915E6

// Transmission power (5 to 20 dBm)
#define LORA_TX_POWER         20

// Spreading Factor (7 to 12)
// Higher = longer range but slower speed
#define LORA_SPREADING_FACTOR 7

// Signal Bandwidth (7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3, 500E3)
#define LORA_SIGNAL_BANDWIDTH 125E3

// Coding Rate (5 to 8)
// Higher = more error correction but slower
#define LORA_CODING_RATE      5

// Sync Word (0x12 = private network, 0x34 = LoRaWAN)
#define LORA_SYNC_WORD        0x12

// ============================
// DATA TRANSMISSION
// ============================

// How often to send periodic updates via LoRa (in milliseconds)
// Set to 0 to send only on entry/exit events
#define TRANSMISSION_INTERVAL 5000

// Device identifier (change for each device in network)
#define DEVICE_ID             "VAS-001"

// ============================
// SERIAL MONITOR
// ============================

// Serial baud rate
#define SERIAL_BAUD_RATE      9600

// Enable debug messages (true = verbose, false = minimal)
#define DEBUG_MODE            true

// ============================
// SENSOR CALIBRATION
// ============================

// Sensor mounting height from floor (in cm)
// For reference only - doesn't affect code
#define SENSOR_HEIGHT         100

// Recommended sensor spacing (in cm)
// For reference only
#define SENSOR_SPACING        15

// ============================
// ADVANCED SETTINGS
// ============================

// Ultrasonic sensor timeout (in microseconds)
// Maximum time to wait for echo
#define ULTRASONIC_TIMEOUT    30000

// Maximum valid distance (in cm)
// Readings above this are considered errors
#define MAX_VALID_DISTANCE    400

// Minimum valid distance (in cm)
// Readings below this are considered errors
#define MIN_VALID_DISTANCE    2

// ============================
// FEATURE TOGGLES
// ============================

// Enable/disable features
#define ENABLE_LORA           true
#define ENABLE_BUZZER         true
#define ENABLE_LEDS           true
#define ENABLE_RESET_BUTTON   true

#endif // CONFIG_H
