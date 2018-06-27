#include <Event.h>
#include <Timer.h>
#include "PMS.h"
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 9, /* data=*/ 10, /* reset=*/ U8X8_PIN_NONE);

int delayval = 500; // delay for half a second


SoftwareSerial mySerial(6, 7); // RX, TX
PMS pms(mySerial);
PMS::DATA data;
int pmValue25;
int pmValue100;
int state;
const int interVal = 500; //측정 인터벌
Timer t;


void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("initializing PMS A003");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  t.every(interVal, readPM);
}


int readPM() {
  u8g2.setFont(u8g2_font_unifont_t_korean1);
  u8g2.setFontDirection(0);
  u8g2.firstPage();  u8g2.setFont(u8g2_font_unifont_t_korean1);
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  if (pms.readUntil(data)) {
    pmValue25 = data.PM_AE_UG_2_5;
    pmValue100 = data.PM_AE_UG_10_0;
  }
  if (pmValue25 > 35 || pmValue100 > 80) {
    Serial.print(pmValue25);
    Serial.print("_BAD_");
    Serial.println(pmValue100);
    state = 0;
  } else {
    Serial.print(pmValue25);
    Serial.print("_GOOD_");
    Serial.println(pmValue100);
    state = 1;
  }



  do {
    u8g2.setCursor(80, 15);
    u8g2.print("공기질");
    if (state == 0) {
      u8g2.setCursor(80, 30);
      u8g2.print("BAD");
    } else {
      u8g2.setCursor(90, 30);
      u8g2.print("GOOD");
    }
    u8g2.setCursor(0, 15);
    u8g2.print("미세");
    u8g2.setCursor(40, 15);
    u8g2.print(pmValue100);
    u8g2.setCursor(0, 30);
    u8g2.print("초미세");
    u8g2.setCursor(55, 30);
    u8g2.print(pmValue25);
  } while ( u8g2.nextPage() );

}

void loop() { // run over and over
  t.update();
}

