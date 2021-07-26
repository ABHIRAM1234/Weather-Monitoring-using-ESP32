#include <Arduino.h>

const int SensorPin = 34;   // moisteure sensor pin

void setup()
{
  Serial.begin(9600);
  //pinMode(SensorPin, INPUT);

}
 void loop()
{
  float sensorValue = analogRead(SensorPin);
  Serial.println(sensorValue);
  delay(3000);

}