/*
     Servo Motor Control - 50Hz Pulse Train Generator
           by Dejan, https://howtomechatronics.com
*/

#define servoPin 9
int count = 0;
void setup() {
  pinMode(servoPin, OUTPUT);
}

void loop() {
   // A pulse each 20ms
   count= count + 1;
    digitalWrite(servoPin, HIGH);
    delayMicroseconds(count); // Duration of the pusle in microseconds
    digitalWrite(servoPin, LOW);
    delayMicroseconds(18550); // 20ms - duration of the pusle
    // Pulses duration: 600 - 0deg; 1450 - 90deg; 2300 - 180deg
    if (count >= 2400)
    {
      count = 0;
    }
 
}