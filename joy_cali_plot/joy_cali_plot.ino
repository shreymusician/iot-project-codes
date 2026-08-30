#define X_PIN 34
#define Y_PIN 35

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  delay(1000);
}

void loop() {

  int x = analogRead(X_PIN);
  int y = analogRead(Y_PIN);

  Serial.print("X:");
  Serial.print(x);

  Serial.print("\tY:");
  Serial.println(y);

  delay(20);
}