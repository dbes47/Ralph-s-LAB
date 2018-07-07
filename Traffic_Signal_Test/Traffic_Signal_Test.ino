
#define LG 6//신호등 릴레이 녹색
#define LY 5//황색
#define LR 4//적색

void setup() {
  pinMode(LG, OUTPUT);
  pinMode(LY, OUTPUT);
  pinMode(LR, OUTPUT); //신호등 제어핀

}
int interval = 1000;

void loop() {

  digitalWrite(LY, HIGH);
  digitalWrite(LR, HIGH);
  digitalWrite(LG, HIGH);
  digitalWrite(LG, LOW);

  delay(interval);
  digitalWrite(LG, HIGH);
  digitalWrite(LR, HIGH);
  digitalWrite(LY, HIGH);
  digitalWrite(LY, LOW);

  delay(interval);
  digitalWrite(LG, HIGH);
  digitalWrite(LY, HIGH);
  digitalWrite(LR, HIGH);
  digitalWrite(LR, LOW);
  delay(interval);

}
