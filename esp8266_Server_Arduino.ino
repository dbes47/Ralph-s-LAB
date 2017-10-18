#include <SoftwareSerial.h>
#define DEBUG true
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT22
DHT mydht(DHTPIN, DHTTYPE);


SoftwareSerial esp8266(10,11); // make RX Arduino line is pin 10, make TX Arduino line is pin 11.
// This means that you need to connect the TX line from the esp to the Arduino's pin 10
// and the RX line from the esp to the Arduino's pin 11


void setup()
{
Serial.begin(9600); // 시리얼 모니터의 baud rate 설정
esp8266.begin(9600); // your esp's baud rate might be different
sendData("AT+RST\r\n",2000,DEBUG); // reset module
sendData("AT+CWMODE=1\r\n",1000,DEBUG); // configure as access point

//sendData("AT+CWLAP\r\n",1000,DEBUG); 
sendData("AT+CWJAP=ralph 5,password\r\n",1000,DEBUG); 


sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80

}

void loop()
{


if(esp8266.available()) { // check if the esp is sending a message 
/*
while(esp8266.available())
{
// The esp has data so display its output to the serial window 
char c = esp8266.read(); // read the next character.
Serial.write(c);
} */



if(esp8266.find("+IPD,")) {
delay(1000);
int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns 
// the ASCII decimal value and 0 (the first decimal number) starts at 48
String tempHum=getDht(DEBUG);
esp8266.find("pin=");
int pinNumber=esp8266.read()-48;

pinMode(pinNumber,OUTPUT);
if(digitalRead(pinNumber)!=HIGH){
digitalWrite(pinNumber,HIGH);
}else{
  digitalWrite(pinNumber,LOW);
}




Serial.print(pinNumber);
Serial.print("\r\n--------------------\r\n");

String webpage = "<h1>LJS DHT22 Server</h1><button scr=?pin=8>LED1</button>";
String cipSend = "AT+CIPSEND=";
cipSend += connectionId;
cipSend += ",";
cipSend +=webpage.length();
cipSend +="\r\n";
sendData(cipSend,1000,DEBUG);
sendData(webpage,1000,DEBUG);
webpage="<a href=?pin=8>LED2</a>\r\n<h2>";
webpage+=tempHum;
webpage+="</h2>";
cipSend = "AT+CIPSEND=";
cipSend += connectionId;
cipSend += ",";
cipSend +=webpage.length();
cipSend +="\r\n";
sendData(cipSend,1000,DEBUG);
sendData(webpage,1000,DEBUG);
String closeCommand = "AT+CIPCLOSE="; 
closeCommand+=connectionId; // append connection id
closeCommand+="\r\n";
sendData(closeCommand,3000,DEBUG);
}
}
}

String sendData(String command, const int timeout, boolean debug) {
String response = "";
esp8266.print(command); // send the read character to the esp8266
long int time = millis();
while( (time+timeout) > millis()) {
while(esp8266.available()) {
// The esp has data so display its output to the serial window 
char c = esp8266.read(); // read the next character.
response+=c;
} 
}
if(debug) {
Serial.print(response);
}
return response;
}

String getDht(boolean debug){
  float h = mydht.readHumidity();
  float t = mydht.readTemperature();
  String response="Hum=";
  response+=h;
  response+="\r\n";
  response+="Temp=";
  response+=t;
  response+="\r\n";
  if(debug){
    Serial.print(response);
  }
  return response;
}

