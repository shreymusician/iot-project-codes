#include <WiFi.h>
#include <esp_now.h>

uint8_t receiverMAC[] = {
  0x00, 0x4B, 0x12, 0x8F, 0xD4, 0x8C
};

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  Serial.print("Controller MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add receiver!");
    return;
  }

  Serial.println("ESP-NOW ready!");
}

void loop() {

  const char message[] = "HELLO FROM CONTROLLER";

  esp_err_t result = esp_now_send(
    receiverMAC,
    (uint8_t *)message,
    sizeof(message)
  );

  if (result == ESP_OK) {
    Serial.println("Message sent!");
  } else {
    Serial.println("Message failed!");
  }

  delay(1000);
}