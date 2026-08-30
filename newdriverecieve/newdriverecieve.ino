#include <WiFi.h>
#include <esp_now.h>

// ================================
// Motor Driver 1 - Left Side
// ================================
#define PWMA 25
#define AIN1 26
#define AIN2 27

// ================================
// Motor Driver 2 - Right Side
// ================================
#define PWMB 32
#define BIN1 33
#define BIN2 13

// Standby pin
#define STBY 14

// Motor speed
const int MOTOR_SPEED = 100;


// =================================
// MOTOR FUNCTIONS
// =================================

void stopMotors() {

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);

  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}


void forward() {

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  analogWrite(PWMA, MOTOR_SPEED);
  analogWrite(PWMB, MOTOR_SPEED);
}


void backward() {

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  analogWrite(PWMA, MOTOR_SPEED);
  analogWrite(PWMB, MOTOR_SPEED);
}


void left() {

  // Left motors backward
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  // Right motors forward
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  analogWrite(PWMA, MOTOR_SPEED);
  analogWrite(PWMB, MOTOR_SPEED);
}


void right() {

  // Left motors forward
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  // Right motors backward
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  analogWrite(PWMA, MOTOR_SPEED);
  analogWrite(PWMB, MOTOR_SPEED);
}


// =================================
// ESP-NOW RECEIVE FUNCTION
// =================================

void onDataReceive(
  const esp_now_recv_info_t *info,
  const uint8_t *data,
  int len
) {

  if (len < 1) {
    return;
  }

  char command = (char)data[0];

  Serial.print("Received: ");
  Serial.println(command);

  switch (command) {

    case 'F':
      Serial.println("Action: FORWARD");
      forward();
      break;

    case 'B':
      Serial.println("Action: BACKWARD");
      backward();
      break;

    case 'L':
      Serial.println("Action: LEFT");
      left();
      break;

    case 'R':
      Serial.println("Action: RIGHT");
      right();
      break;

    case 'P':
      Serial.println("Action: PRESSED");
      stopMotors();
      break;

    case 'S':
      Serial.println("Action: STOP");
      stopMotors();
      break;

    default:
      Serial.println("Unknown command");
      stopMotors();
      break;
  }
}


// =================================
// SETUP
// =================================

void setup() {

  Serial.begin(115200);

  // Motor pins
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(STBY, OUTPUT);

  // Enable TB6612FNG
  digitalWrite(STBY, HIGH);

  // Start with motors stopped
  stopMotors();

  // Wi-Fi in station mode
  WiFi.mode(WIFI_STA);

  Serial.println();
  Serial.println("ROVER ESP32");
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {

    Serial.println("ESP-NOW initialization failed!");

    return;
  }

  // Register receive callback
  esp_now_register_recv_cb(onDataReceive);

  Serial.println("ESP-NOW receiver ready!");
}


// =================================
// LOOP
// =================================

void loop() {

  // Nothing required here.
  // Commands are handled by ESP-NOW callback.

}