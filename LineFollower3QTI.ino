#include <Servo.h>                           // Include servo library

Servo servoLeft;                             // Declare left and right servos
Servo servoRight;

int QTILeft = 4;                           // Attach left QTI sensor to pin 2
int QTIMiddle = 3;                         // Attach middle QTI sensor to pin 3
int QTIRight = 2;                          // Attach right QTI sensor to pin 4


void setup()                                 // Built-in initialization block
{
  Serial.begin(9600);                        // Sensor sampling rate
}  


void loop() 
{
  Stop(50);
  int LeftCol = GetColour(QTILeft);                              // Take initial readings
  int MidCol = GetColour(QTIMiddle);
  int RightCol = GetColour(QTIRight);

  if (LeftCol == 0 && MidCol == 0 && RightCol == 0){               // All 3 Sensors white - willing to bet its a junction
    Forward(100);
  }

  else if (LeftCol == 0 && MidCol == 0 && RightCol == 1){         // Black background with white line - left sensor has hit the line
    SeekLeft(LeftCol, RightCol, QTILeft, QTIRight);             // Rotates left until sensors read black again
    Forward(50);
  }

  else if (LeftCol == 0 && MidCol == 1 && RightCol == 0){         // White background with black line - all readings correct, continue forward
    Forward(100);
  }

  else if (LeftCol == 0 && MidCol == 1 && RightCol == 1){         // White background with black line - right sensor has hit the line
    SeekRight(LeftCol, RightCol, QTILeft, QTIRight);            // Rotates right until both sensors read white again
    Forward(50);
  }

  else if (LeftCol == 1 && MidCol == 0 && RightCol == 0){         // Black background with white line - right sensor has hit the line
    SeekRight(LeftCol, RightCol, QTILeft, QTIRight);            // Rotates right until both sensors read white again
    Forward(50);
  }

  else if (LeftCol == 1 && MidCol == 0 && RightCol == 1){         // Black background with white line - all readings correct, continue forward
    Forward(100);
  }

  else if (LeftCol == 1 && MidCol == 1 && RightCol == 0){         // White background with black line - left sensor has hit the line
    SeekLeft(LeftCol, RightCol, QTILeft, QTIRight);            // Rotates left until both sensors read white again
    Forward(50);
  }

  else if (LeftCol == 1 && MidCol == 1 && RightCol == 1){         // All 3 sensors black - willing to bet its a junction
    Forward(100);
  }

}

long RCTime(int sensorIn){
  long duration = 0;
  pinMode(sensorIn, OUTPUT); // Make pin OUTPUT
  digitalWrite(sensorIn, HIGH); // Pin HIGH (discharge capacitor)
  delay(1); // Wait 1ms
  pinMode(sensorIn, INPUT); // Make pin INPUT
  digitalWrite(sensorIn, LOW); // Turn off internal pullups
  while(digitalRead(sensorIn)){ // Wait for pin to go LOW
    duration++;
  }
  return duration;
}

int GetColour(int QTIPin){
  long Reading = RCTime(QTIPin);
  if (Reading > 200){
    return 0;       //White
  }
  if (Reading < 200){
    return 1;       //Black
  }
}


void Forward(int distance){
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoLeft.writeMicroseconds(1625);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(distance);                           // Control distance moved
  servoLeft.detach();
  servoRight.detach();
}

void Reverse(int distance){
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12 
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1625);        // Right wheel counterclockwise
  delay(distance);                           // Control distance moved
  servoLeft.detach();
  servoRight.detach();
}

void RotateLeft(int angle){
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoLeft.writeMicroseconds(1300);         // Left wheel clockwise
  servoRight.writeMicroseconds(1300);        // Right wheel clockwise
  delay(angle);                              // Control angle of rotation
  servoLeft.detach();
  servoRight.detach();
}

void RotateRight(int angle){
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoLeft.writeMicroseconds(1700);         // Left wheel counterclockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
  delay(angle);                              // Control angle of rotation
  servoLeft.detach();
  servoRight.detach();
}

void Stop(int time){
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  servoLeft.writeMicroseconds(1500);         // Stop Left Servo
  servoRight.writeMicroseconds(1500);        // Stop Right Servo
  delay(time);
  servoLeft.detach();
  servoRight.detach();
}

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
