int servo=2;
int max=180;
int min=0;
float val=0;
void setup() {
  // put your setup code here, to run once:
pinMode(servo,OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  val++;
  float valP=map(val,0,1024,1,2);
  constrain(valP,1,2);
digitalWrite(servo, HIGH);
delay(valP);
digitalWrite(servo,LOW);
delay(20-valP);
Serial.println(valP);
}
