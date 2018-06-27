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

int delayval = 2000; // delay for half a second
int swap_flag = 12;
///////////////////////////////////led parts
const int dim = 50; //밝기값

//  0x00606e6a6a6e6000,미
//  0x007e1866187e1800 초
//  0x18e78181e72424e7 2.5
//0x0001017f7f110100

const uint64_t IMAGES_SOURCE[] = { //number figure 64bit array
  0x003e7f51497f3e00,//0
  0x0001017f7f110100,//1
  0x0031794945672300,//2
  0x00367f4949632200,//3
  0x00047f7f24140c00,//4
  0x004e5f5151737200,//5
  0x00266f49497f3e00,//6
  0x0060785f47606000,//7
  0x00367f49497f3600,//8
  0x003e7f49497b3200,//9
  0x0025297777292500,//초
  0x0000000000000000,//공백
  0x7e7e003c24243c00//미
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
  0x0000000000000000,
  0x0000000000000000,
  0x0000000000000000,//nothing
  0x0000000000000000
};

uint64_t IMAGES[] = {//실제 화면에 뿌려지게될 배열
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
const int interVal = 500; //측정 인터벌
int colorValR = 0; //rgb 색상값 변
int colorValG = 0;
int colorValB = 0;
Timer t;
Timer flag;


void setup() {

  pixels.begin();
  flipBitArray();//숫자를 대칭작업하여 제대로 보이게 하기

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("initializing PMS A003");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  t.every(interVal, readPM);
  flag.every(5000,changeFlag);
}
void changeFlag(){//미세와 초미세 플래그 반전
  if(swap_flag==12){
    swap_flag=10;
  }else{
    swap_flag=12;
  }
}


int readPM() {
  if (pms.readUntil(data)) {
    pmValue25 = data.PM_AE_UG_2_5;
    pmValue100 = data.PM_AE_UG_10_0;
  }
  if (pmValue25 > 35 || pmValue100 > 80) {
    Serial.print(pmValue25);
    Serial.print("_BAD_");
    Serial.println(pmValue100);
    colorValR = dim;
    colorValG = 0;
    colorValB = 0;
  } else {
    Serial.print(pmValue25);
    Serial.print("_GOOD_");
    Serial.println(pmValue100);

    colorValR = 0;
    colorValG = 0;
    colorValB = dim;
  }
  cleanIMAGES();
  //미세먼지 농도에 따른 수치 표시하기
  if (swap_flag == 12) {
    if (pmValue25 > 99 && pmValue25 < 999) {
      setIMAGES(10, pmValue25 / 100, (pmValue25 % 100) / 10, pmValue25 % 10);
    } else if (pmValue25 > 9 && pmValue25 < 100) {
      setIMAGES(10, 11, pmValue25 / 10, pmValue25 % 10);
    } else if (pmValue25 < 10) {
      setIMAGES(10, 11, 11, pmValue25);
    }
  } else if(swap_flag==10){
    if (pmValue100 > 99 && pmValue100 < 999) {
      setIMAGES(12, pmValue100 / 100, (pmValue100 % 100) / 10, pmValue100 % 10);
    } else if (pmValue100 > 9 && pmValue100 < 100) {
      setIMAGES(12, 11, pmValue100 / 10, pmValue100 % 10);
    } else if (pmValue100 < 10) {
      setIMAGES(12, 11, 11, pmValue100);
    }
  }
  //setIMAGES(random(9)+1, random(9)+1, random(9)+1, random(9)+1);
}

void loop() { // run over and over
  flag.update();
  t.update();
  showNumber();
}

void flipBitArray() { //원점 대칭 후 좌우 대칭

  for (int _n = 0; _n < 13; _n++) {

    temp = temp & bitCleaner;
    temp2 = temp2 & bitCleaner;//임시 비트들을 지우기

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
  IMAGES[0] = IMAGES[0] | FLIPPED[_num1];
  IMAGES[1] = IMAGES[1] | FLIPPED[_num2];
  IMAGES[2] = IMAGES[2] | FLIPPED[_num3];
  IMAGES[3] = IMAGES[3] | FLIPPED[_num4];
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
      pixels.setPixelColor(i, pixels.Color(0, 30, 0));
    } else {
      pixels.setPixelColor(i, pixels.Color(colorValR, colorValG, colorValB));
    }
  }
  pixels.show();
}

