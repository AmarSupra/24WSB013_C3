#include <Servo.h>                           // Include servo library

Servo servoLeft;                             // Declare left and right servos
Servo servoRight;

int QTILeft = 4;                           // Attach left QTI sensor to pin 4
int QTIMiddle = 5;                         // Attach middle QTI sensor to pin 5
int QTIRight = 2;                          // Attach right QTI sensor to pin 2

int LeftBackground = 0;                    // Initialise QTI readings to 0 or white
int RightBackground = 0; 

unsigned long PreviousTime = 0; // Last timestamp for checking
unsigned long CheckInterval = 2000; // Time interval to check (in milliseconds)
int ChangeCount = 0;       // Counter for state changes
int MiddlePrevious = 0;  // Stores last colour in middle QTI
const int ChangeThreshold = 4;  // Threshold for rapid changes (in transitions per second)

void setup()                                 // Built-in initialization block
{
  Serial.begin(9600);
}  



void loop()                                                         // Main loop auto-repeats
{          
  Stop(50);
  int LeftColour = GetColour(QTILeft);                              // Take initial readings
  int RightColour = GetColour(QTIRight);
  int MiddleColour = GetColour(QTIMiddle);

  if (MiddleColour != MiddlePrevious)                               //Code for detecting changing colours in middle QTI for rapid change detection
  {
    ChangeCount++;                                                  //Increments Change counter
    MiddlePrevious = MiddleColour;                                  //Updates previous colour
  }


  // Check if the interval has elapsed
  unsigned long CurrentTime = millis();
  if (CurrentTime - PreviousTime >= CheckInterval) {
    // Calculate the number of changes per second
    int changesPerSecond = ChangeCount;

    // Print the results
    Serial.print("Changes per second: ");
    Serial.println(changesPerSecond);

    // Check if the changes exceed the threshold
    if (changesPerSecond > ChangeThreshold) {
      RotateRight(5000);
    }

    // Reset the counter and timestamp
    ChangeCount = 0;
    PreviousTime = CurrentTime;
  }

  

  if (LeftColour == RightColour)                                    // Same colour, QTIs are over both sides of the line
  {
    Forward(100);                                                     // Moves forwards
    LeftBackground = LeftColour;                                    // Since same colour, updates background readings
    RightBackground = RightColour;
  }
  else                                                              // Whenever QTI readings are not equal, code below will run
  {
    if (LeftBackground == 0 && RightBackground == 0)                // For a white background with a black line
    {
      if (LeftColour == 1 && RightColour == 0)                      // Left sensor is on the line and right is off the line
      {
        SeekLeft(LeftColour, RightColour, QTILeft, QTIRight);       // Rotates left until both sensors read white again
        Forward(50);
      }
      else                                                          // Else assumes that only other condition is left sensor off the line and right sensor on the line
      {
        SeekRight(LeftColour, RightColour, QTILeft, QTIRight);      // Rotates right until both sensors read white again
        Forward(50);
      }
    }

    else if (LeftBackground == 1 && RightBackground == 1)           // For a black background with a white line
    {
      if (LeftColour == 1 && RightColour == 0)                      // Left sensor off the line and right is on the line
      {
        SeekRight(LeftColour, RightColour, QTILeft, QTIRight);      // Rotates right until sensors read black again
        Forward(50);
      }
      else                                                          // Else assumes the only other condition is left sensor on the line and right sensor off the line
      {
        SeekLeft(LeftColour, RightColour, QTILeft, QTIRight);       // Rotates left until sensors read black again
        Forward(50);
      }
    }
    else
    {
      Serial.println("Unexpected difference in background colours");  // For debugging purposes, backgrounds should never be unequal
    }
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
  if (Reading < 200){         //values for comparison should be adjusted based off of readings
    return 0;       //White
  }
  if (Reading > 200){
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
  delay(15);
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
