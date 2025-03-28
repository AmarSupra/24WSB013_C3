#include <SoftwareSerial.h>
#include <Wire.h>
#include "rgb_lcd.h"

SoftwareSerial mySerial(7, 8);  //Rx=7, Tx=8 wire it opposite

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

String receivedData;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    lcd.setRGB(colorR, colorG, colorB);

}

void loop() {
  if (mySerial.available()) {
    receivedData = mySerial.readStringUntil('\n'); // Read entire data until newline character
    Serial.println(receivedData); // Print the data to Serial Monitor
  }

  lcd.setCursor(0,0);
  lcd.print(receivedData);

}

