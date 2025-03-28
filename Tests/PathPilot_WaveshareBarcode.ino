#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8);  //Rx=7, Tx=8 wire it opposite

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  if (mySerial.available()) {
    String receivedData = mySerial.readStringUntil('\n'); // Read entire data until newline character
    Serial.println(receivedData); // Print the data to Serial Monitor
  }
}