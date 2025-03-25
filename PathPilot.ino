////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        PATH PILOT - LINE FOLLOWING AND BARCODE SCANNING                                            //
//                                                                                                                                    //
//           CIRCUITRONICS - COMPANY C GROUP 3 - AMAR SUPRA, SHANIA MIRANDA, KAVIYAN ELANCHEZHIAN, BEN BARBER                         //
//                                                                                                                                    //
//           We have designed a Boe-Bot attachtment to follow a line and scan the barcode at the end of it whilst outputting          //
//           the result. The solution is based on QTI sensors to follow a line by making colour comparisons and we use a              //
//           Waveshare barcode scanner component to scan the barcode. Our Bot is capable of handling junctions and colour             //
//           changes in a line.                                                                                                       //
//                                                                                                                                    //
//           Before running check and adjust accordingly : Pin assignments, Light readings, Movement parameters                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LIBRARIES ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Servo.h>                           // Servo library
#include <SoftwareSerial.h>                  // Serial library for barcode outputs
#include <Wire.h>                            // Library for I2C connection to LCD
#include "rgb_lcd.h"                         // LCD library

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SENSOR DECLARATIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Servo servoLeft;                             // Declare left and right servos
Servo servoRight;

SoftwareSerial BarcodeSerial(7, 8);          // Declare up serial for barcode sensor, Recieves on pin 7 (RX), Transmits on pin 8 (TX)

int QTILeft = 4;                             // Attach left QTI sensor to pin 4
int QTIRight = 2;                            // Attach right QTI sensor to pin 2

rgb_lcd lcd;                                 // Declare LCD

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SETUP VARIABLES ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String BarcodeNumber = "";                  // Initialise Variable to hold barcode number

int LeftBackground = 0;                     // Initialise QTI readings to 0 or white
int RightBackground = 0; 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SETUP - Built-in Code that runs before loop /////////////////////////////////////////////////////////////////////////////////////////////
void setup()                                 
{
  Serial.begin(9600);                        // Setup Serial communication rates
  BarcodeSerial.begin(9600);

  lcd.begin(16, 2);                          // Setup LCD columns, rows and colour
  lcd.setRGB(255, 0, 0);

  LCDUpdate(Line Following);
}  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN LOOP - Line following and barcode scanning main code ///////////////////////////////////////////////////////////////////////////////
void loop()                                                        
{          
  Stop(50);                                                         // Stop to take readings and make checks

  int LeftColour = GetColour(QTILeft);                              // Take QTI readings
  int RightColour = GetColour(QTIRight);

  if (BarcodeSerial.available()) {
    BarcodeNumber = BarcodeSerial.readStringUntil('\n');            // Read barcode number from serial output
  }

  if (BarcodeNumber != "")                                          // Barcode has been scanned - run final code
  {
    LCDUpdate(BarcodeNumber);                                       // Output barcode number

    Forward(1000);                                                  // Drive over barcode to finish
    lcd.setCursor(0,1);                                             // Update LCD to show current status
    lcd.print("TASK COMPLETE");
    RotateRight(250);                                               // Dance ?
    while (1 == 1){}                                                // Infinite loop does nothing - Job is complete
  }
  
  // QTIs read same colour - Move forwards and update backgrounds
  if (LeftColour == RightColour)                                    // QTIs are over the background with the line between them        
  {
    Forward(100);                                                   
    LeftBackground = LeftColour;                                    
    RightBackground = RightColour;
  }

  // Whenever QTI readings are not equal, code below will run
  else                                                              
  {
    if (LeftBackground == 0 && RightBackground == 0)                // White background, Black line
    {
      if (LeftColour == 1 && RightColour == 0)                      // Left sensor is on the line and right is off the line
      {
        SeekLeft(LeftColour, RightColour, QTILeft, QTIRight);       // Rotates left until both sensors read white again
        Forward(50);
      }
      else                                                          // Left sensor off the line and right sensor on the line
      {
        SeekRight(LeftColour, RightColour, QTILeft, QTIRight);      // Rotates right until both sensors read white again
        Forward(50);
      }
    }

    else                                                            // Black background(1), White line
    {
      if (LeftColour == 1 && RightColour == 0)                      // Left sensor off the line and right is on the line
      {
        SeekRight(LeftColour, RightColour, QTILeft, QTIRight);      // Rotates right until sensors read black again
        Forward(50);
      }
      else                                                          // Left sensor on the line and right sensor off the line
      {
        SeekLeft(LeftColour, RightColour, QTILeft, QTIRight);       // Rotates left until sensors read black again
        Forward(50);
      }
    }
    
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS & PROCEDURES /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The RCTime function gives the reading from a QTI sensor
long RCTime(int sensorIn){
  long duration = 0;
  pinMode(sensorIn, OUTPUT);                                     // Make pin OUTPUT
  digitalWrite(sensorIn, HIGH);                                  // Pin HIGH (discharge capacitor)
  delay(1);                                                      // Wait 1ms
  pinMode(sensorIn, INPUT);                                      // Make pin INPUT
  digitalWrite(sensorIn, LOW);                                   // Turn off internal pullups
  while(digitalRead(sensorIn)){                                  // Wait for pin to go LOW
    duration++;
  }
  return duration;
}

// The GetColour function converts the QTI reading to a white or black value
int GetColour(int QTIPin){
  long Reading = RCTime(QTIPin);
  if (Reading < 200){                                            // Values for comparison should be adjusted based off of readings
    return 0;                                                    // White = 0
  }
  if (Reading > 200){
    return 1;                                                    // Black = 0
  }
}

// The LCDUpdate function updates the LCD display
void LCDUpdate(String Message){
  lcd.clear();                                                   // LCD resets and outputs barcode number
  lcd.setCursor(0,0);
  lcd.print(Message);
}

// Movement Functions
// 1700 = Full Speed anticlockwise
// 1300 = Full Speed clockwise
// 1500 = Stop
// Linear Zones: 1700 - 1600, 1300 - 1400    Non-Linear Zones: 1600-1500, 1400 - 1500

void Forward(int distance){
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoLeft.writeMicroseconds(1625);         // Left wheel anticlockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(distance);                           // Control distance moved
  servoLeft.detach();
  servoRight.detach();
}

void RotateLeft(int angle){
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoLeft.writeMicroseconds(1375);         // Left wheel clockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(angle);                              // Control angle of rotation
  servoLeft.detach();
  servoRight.detach();
}

void RotateRight(int angle){
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoLeft.writeMicroseconds(1625);         // Left wheel anticlockwise
  servoRight.writeMicroseconds(1700);        // Right wheel anticlockwise
  delay(angle);                              // Control angle of rotation
  servoLeft.detach();
  servoRight.detach();
}

void Stop(int time){
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoLeft.writeMicroseconds(1500);         // Stop Left Servo
  delay(7);                                  // Delay because of motor imbalance
  servoRight.writeMicroseconds(1500);        // Stop Right Servo
  delay(time);
  servoLeft.detach();
  servoRight.detach();
}

// The SeekLeft function is used to regain the line when the bot needs to turn back onto it
void SeekLeft(int LeftColour, int RightColour, int LeftPin, int RightPin){
  int lc = LeftColour;
  int rc = RightColour;
  while (lc != rc)
  {
    RotateLeft(25);
    lc = GetColour(LeftPin);
    rc = GetColour(RightPin);    
  }
}

// The SeekRight function is used to regain the line when the bot needs to turn back onto it
void SeekRight(int LeftColour, int RightColour, int LeftPin, int RightPin){
  int lc = LeftColour;
  int rc = RightColour;
  while (lc != rc)
  {
    RotateRight(25);
    lc = GetColour(LeftPin);
    rc = GetColour(RightPin);    
  }
}

