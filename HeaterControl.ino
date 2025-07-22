#include <math.h>
// Pin Definitions (your specified pins)
#define TEMP_SENSOR_PIN A0
#define HEATER_LED_PIN 3
#define STATUS_LED_PIN 4
#define BUZZER_PIN 5

// Temperature Thresholds
#define START_HEATING_TEMP 35
#define STABILIZING_TEMP   40
#define TARGET_TEMP        45
#define OVERHEAT_TEMP      50

// System States
enum State {IDLE, HEATING, STABILIZING, TARGET_REACHED, OVERHEAT};
State systemState = IDLE;

// Timer
unsigned long previousMillis = 0;
const long interval = 2000; // 2 seconds

void setup() {
  Serial.begin(9600);
  pinMode(HEATER_LED_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

// Function to read temperature from NTC Thermistor
float readTemperature() {
  int analogValue = analogRead(TEMP_SENSOR_PIN);
  
  float resistance = (1023.0 / analogValue) - 1.0;
  resistance = 10000.0 / resistance; // 10kΩ resistor

  float steinhart;
  steinhart = resistance / 10000.0;
  steinhart = log(steinhart);
  steinhart /= 3950.0;               // Beta value
  steinhart += 1.0 / (25.0 + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;

  return steinhart;
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float temp = readTemperature();
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");

    updateSystemState(temp);
    controlHeaterSystem();
  }
}

void updateSystemState(float temp) {
  if (temp >= OVERHEAT_TEMP) {
    systemState = OVERHEAT;
  } else if (temp >= TARGET_TEMP) {
    systemState = TARGET_REACHED;
  } else if (temp >= STABILIZING_TEMP) {
    systemState = STABILIZING;
  } else if (temp >= START_HEATING_TEMP) {
    systemState = HEATING;
  } else {
    systemState = IDLE;
  }
}

void controlHeaterSystem() {
  switch(systemState) {
    case IDLE:
      digitalWrite(HEATER_LED_PIN, LOW);
      digitalWrite(STATUS_LED_PIN, LOW);
      noTone(BUZZER_PIN);
      Serial.println("State: IDLE");
      break;

    case HEATING:
      digitalWrite(HEATER_LED_PIN, HIGH);
      digitalWrite(STATUS_LED_PIN, LOW);
      noTone(BUZZER_PIN);
      Serial.println("State: HEATING");
      break;

    case STABILIZING:
      digitalWrite(HEATER_LED_PIN, HIGH);
      digitalWrite(STATUS_LED_PIN, LOW);
      noTone(BUZZER_PIN);
      Serial.println("State: STABILIZING");
      break;

    case TARGET_REACHED:
      digitalWrite(HEATER_LED_PIN, LOW);
      digitalWrite(STATUS_LED_PIN, LOW);
      tone(BUZZER_PIN, 1000, 500); // 1kHz short beep for 500ms
      Serial.println("State: TARGET REACHED");
      break;

    case OVERHEAT:
      digitalWrite(HEATER_LED_PIN, LOW);
      digitalWrite(STATUS_LED_PIN, HIGH);
      tone(BUZZER_PIN, 2000); // Continuous alarm at 2kHz
      Serial.println("State: OVERHEAT! Heater OFF, ALERT ON");
      break;
  }
}
