void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(GetColour(4));
  Serial.println(GetColour(2));

}

void loop() {

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
  if (Reading < 200){
    return 0;       //White
  }
  if (Reading > 200){
    return 1;       //Black
  }
}