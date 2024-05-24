/*
  Fading

  This example shows how to fade an LED using the analogWrite() function.

  The circuit:
  - LED attached from digital pin 9 to ground through 220 ohm resistor.

  created 1 Nov 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Fading
*/

int ledPin = 9;  // LED connected to digital pin 9
int LEDval = 0;
void setup() {
  // nothing happens in setup
}

void loop() {
  int sensorValue = analogRead(A0);
  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += sensorValue/100) {
    // sets the value (range from 0 to 255):
    int sensorValue = analogRead(A0);
    analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= sensorValue/100) {
    // sets the value (range from 0 to 255):
    int sensorValue = analogRead(A0);
    analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}
