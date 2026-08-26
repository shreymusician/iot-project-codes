#include <ESP_I2S.h>

I2SClass I2S;

#define SCK_PIN 22
#define WS_PIN  33
#define SD_PIN  35

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println(" INMP441 FINAL TEST");
  Serial.println("==============================");

  I2S.setPins(
    SCK_PIN,
    WS_PIN,
    -1,
    SD_PIN
  );

  if (!I2S.begin(
        I2S_MODE_STD,
        16000,
        I2S_DATA_BIT_WIDTH_32BIT,
        I2S_SLOT_MODE_MONO,
        I2S_STD_SLOT_LEFT
      )) {

    Serial.println("I2S START FAILED!");
    Serial.print("I2S error: ");
    Serial.println(I2S.lastError());

    while (true);
  }

  Serial.println("I2S STARTED!");

  delay(1000);

  Serial.println();
  Serial.println("Recording 1 second...");
  Serial.println("SPEAK / CLAP NOW!");

  size_t wavSize = 0;

  uint8_t *wavData = I2S.recordWAV(
    1,
    &wavSize
  );

  Serial.println();
  Serial.println("Recording finished.");

  if (wavData == nullptr) {

    Serial.println("WAV RECORDING FAILED!");

    Serial.print("I2S last error: ");
    Serial.println(I2S.lastError());

    return;
  }

  Serial.println("WAV RECORDING SUCCESS!");

  Serial.print("WAV size: ");
  Serial.println(wavSize);

  free(wavData);
}

void loop() {
}