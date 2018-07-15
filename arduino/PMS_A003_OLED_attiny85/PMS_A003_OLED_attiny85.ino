#include "PMS.h"
#include <SoftSerial.h>
#include <Tiny4kOLED.h>

SoftSerial mySerial(5, 4); // RX, TX
PMS pms(mySerial);
PMS::DATA data;
int a = 0;

void setup() {
  oled.begin();
  oled.clear();
  oled.setFont(FONT16X32);
  mySerial.begin(9600);
}

void loop() {
  oled.clear();
  oled.setCursor(0, 0);
  oled.on();
  oled.print(a); 6
  a++;
  if (pms.readUntil(data)) {
    oled.on();
    oled.print(data.PM_AE_UG_1_0);
  }
  delay(200);
}

