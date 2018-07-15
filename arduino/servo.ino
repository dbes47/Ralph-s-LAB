int servo=2;
float degree=0;
int rpm=20;
float waitTime=rpm*9;
int a=0;
bool maiCaused=false;
bool done=false;
void setup() {
pinMode(servo,OUTPUT);
Serial.begin(9600);
initServo();
}

void loop() {
  if(maiCaused==false){
movingServo(120);
  }else if(done==false){
    movingServo(90);
    done=true;
  }
}

void initServo(){
  int elapsedTime=millis();
 do{
  digitalWrite(servo,HIGH);
  delay(0);
  digitalWrite(servo,LOW);
  delay(rpm);
}while(millis()-elapsedTime<800);
Serial.println("done");
}

void movingServo(float _degree){
  int elapsedTime=millis();  
degree=abs(degree-_degree);
float _delta=degree/90*1000;
Serial.println(a);

do{
digitalWrite(servo, HIGH);
delayMicroseconds(_delta);
digitalWrite(servo,LOW);
delayMicroseconds(rpm*1000);  
}while(millis()-elapsedTime<800);
maiCaused=true;
}
