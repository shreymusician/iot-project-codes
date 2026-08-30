#include <WiFi.h>
#include <esp_now.h>
#include <Joystick.h>

// --------------------
// Joystick
// --------------------
Joystick joystick(34, 35, 4);

// --------------------
// Rover ESP32 MAC
// --------------------
uint8_t roverMAC[] = {
  0x00, 0x4B, 0x12, 0x8F, 0xD4, 0x8C
};

// --------------------
// ESP-NOW peer
// --------------------
esp_now_peer_info_t peerInfo;

// --------------------
// Setup
// --------------------
void setup() {

  Serial.begin(115200);

  // Initialize joystick
  joystick.begin();

  // Initialize Wi-Fi
  WiFi.mode(WIFI_STA);

  Serial.println("Controller ESP32");
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  // Configure receiver
  memset(&peerInfo, 0, sizeof(peerInfo));

  memcpy(peerInfo.peer_addr, roverMAC, 6);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add rover
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add rover!");
    return;
  }

  Serial.println("ESP-NOW ready!");
}

// --------------------
// Send command
// --------------------
void sendCommand(char command) {

  esp_err_t result = esp_now_send(
    roverMAC,
    (uint8_t *)&command,
    sizeof(command)
  );

  if (result == ESP_OK) {
    Serial.print("Sent: ");
    Serial.println(command);
  }
  else {
    Serial.println("Send failed!");
  }
}

// --------------------
// Main loop
// --------------------
void loop() {

  String state = joystick.getState();

  char command;

  if (state == "FORWARD") {
    command = 'F';
  }

  else if (state == "BACKWARD") {
    command = 'B';
  }

  else if (state == "LEFT") {
    command = 'L';
  }

  else if (state == "RIGHT") {
    command = 'R';
  }

  else if (state == "PRESSED") {
    command = 'P';
  }

  else {
    command = 'S';   // NORMAL → STOP
  }

  sendCommand(command);

  delay(100);
}