#define X_PIN 34
#define Y_PIN 35
#define SW_PIN 4

void setup() {
  Serial.begin(115200);

  pinMode(SW_PIN, INPUT_PULLUP);

  analogReadResolution(12);
}

void loop() {

  int x = analogRead(X_PIN);
  int y = analogRead(Y_PIN);
  int sw = digitalRead(SW_PIN);

  // Current interpretation
  String state;

  if (sw == LOW) {
    state = "P";
  }
  else if (x > 3500 && y > 3500) {
    state = "R";
  }
  else if (x > 3500 && y < 3500) {
    state = "F";
  }
  else if (x < 1500) {
    state = "B";
  }
  else if (y < 1500) {
    state = "L";
  }
  else {
    state = "NORMAL";
  }

  // Print everything
  Serial.print("X: ");
  Serial.print(x);

  Serial.print(" | Y: ");
  Serial.print(y);

  Serial.print(" | Interpretation: ");
  Serial.println(state);

  delay(100);
}