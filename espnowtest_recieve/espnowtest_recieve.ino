#include <WiFi.h>
#include <esp_now.h>

void onDataReceive(
  const esp_now_recv_info_t *info,
  const uint8_t *data,
  int len
) {
  Serial.print("Received: ");

  for (int i = 0; i < len; i++) {
    Serial.print((char)data[i]);
  }

  Serial.println();
}

void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  Serial.print("Receiver MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed!");
    return;
  }

  esp_now_register_recv_cb(onDataReceive);

  Serial.println("ESP-NOW receiver ready!");
}

void loop() {
}