#include <Servo.h>                           // Include servo library

Servo servoLeft;                             // Declare left and right servos
Servo servoRight;
 
void setup()                                 // Built-in initialization block
{
 
}  


void loop()                                  // Main loop auto-repeats
{          
  Forward(500);
  Stop(100);

}

//1700 = counterclockwise
//1300 = clockwise
//1500 = stop

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
  servoLeft.writeMicroseconds(1375);         // Left wheel clockwise
  servoRight.writeMicroseconds(1700);        // Right wheel counterclockwise
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
  servoLeft.writeMicroseconds(1625);         // Left wheel counterclockwise
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
