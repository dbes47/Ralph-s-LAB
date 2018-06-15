#include <Event.h>
#include <Timer.h>
#include "PMS.h"
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


#define PIN            2 //led를 i2c로 통신할 포트 설정
#define NUMPIXELS      256
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); // led 객체를 선언

int delayval = 50; // delay for half a second

///////////////////////////////////led parts
const uint64_t IMAGES_SOURCE[] = { //number figure 64bit array
  0x0001017f7f110100,//0
  0x0031794945672300,//1
  0x00367f4949632200,//2
  0x00047f7f24140c00,//3
  0x004e5f5151737200,//4
  0x00266f49497f3e00,//5
  0x0060785f47606000,//6
  0x00367f49497f3600,//7
  0x003e7f49497b3200,//8
  0x003e7f51497f3e00//9
};

uint64_t temp = 0x0000000000000000;
uint64_t temp2 = 0x0000000000000000;
const uint64_t bitCleaner = 0x0000000000000000;//비트 삭제용 비트

uint64_t FLIPPED[] = {//대칭시키기 위한 빈 64비트 배열
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000
};

uint64_t IMAGES[] = {
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000,
};

//////////////led parts end//////////////////////////





SoftwareSerial mySerial(10, 11); // RX, TX
PMS pms(mySerial);
PMS::DATA data;
int pmValue25;
int pmValue100;
const int interVal=2000;//측정 인터벌
int colorValR=0;//rgb 색상값 변
int colorValG=0;
int colorValB=0;
Timer t;


void setup() {

  pixels.begin();
  flipBitArray();//숫자를 대칭작업하여 제대로 보이게 하기
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("initializing PMS A003");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  t.every(interVal,readPM);
}

int readPM(){
  if(pms.readUntil(data)){
      pmValue25=data.PM_AE_UG_2_5;
      pmValue100=data.PM_AE_UG_10_0;
  }
  if(pmValue25>35 || pmValue100>80){
    Serial.print(pmValue25);
    Serial.print("_BAD_");
    Serial.println(pmValue100);
    colorValR=10;
    colorValG=0;
    colorValB=0;
  }else{
    Serial.print(pmValue25);
    Serial.print("_GOOD_");
    Serial.println(pmValue100);

    colorValR=0;
    colorValG=0;
    colorValB=10;
  }
  cleanIMAGES();
  setIMAGES(random(9)+1, random(9)+1, random(9)+1, random(9)+1);
}

void loop() { // run over and over
  t.update();
  showNumber();
}

void flipBitArray() {//원점 대칭 후 좌우 대칭

  for (int _n = 0; _n < 10; _n++) {
    temp = temp & bitCleaner;
    temp2 = temp2 & bitCleaner;
    for (int j = 0; j < 64; j++) {
      temp = IMAGES_SOURCE[_n] << j;
      temp = temp >> 63;
      temp = temp << j;
      temp2 = temp2 | temp;
    }
    for (int j = 0; j < 8; j++) {
      temp = temp2 << j * 8;
      temp = temp >> 56;
      temp = temp << j * 8;
      FLIPPED[_n] = FLIPPED[_n] | temp;
    }
  }
}

void setIMAGES(int _num1, int _num2, int _num3, int _num4) {
  IMAGES[0] = IMAGES[0] | FLIPPED[_num1-1];
  IMAGES[1] = IMAGES[1] | FLIPPED[_num2-1];
  IMAGES[2] = IMAGES[2] | FLIPPED[_num3-1];
  IMAGES[3] = IMAGES[3] | FLIPPED[_num4-1];
}

void cleanIMAGES() {
  for (int i = 0; i < 4; i++) {
    IMAGES[i] = IMAGES[i] & bitCleaner;
  }//전체 화면을 초기화
}

void showNumber() {
  int _i = 0;
  for (int i = 0; i < 256; i++) {
    if ((i / 8) % 2 == 0) {
      //  Serial.println(i);
      _i = i;
    } else {
      //  Serial.println((i / 8 + 1) * 8 - i % 8 - 1);
      _i = (i / 8 + 1) * 8 - i % 8 - 1;
    }
    if (bitRead(IMAGES[i / 64], _i % 64) == 1) {
      pixels.setPixelColor(i, pixels.Color(5, 5, 0));
    } else {
      pixels.setPixelColor(i, pixels.Color(colorValR, colorValG, colorValB));
    }
  }
  pixels.show();
}

