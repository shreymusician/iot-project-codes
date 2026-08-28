#define PWMA 25
#define AIN1 26
#define AIN2 27
#define STBY 14

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(PWMA, HIGH);
}

void loop() {
}