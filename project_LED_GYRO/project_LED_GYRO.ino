//loads libarys of neopixel and gyro
#include <Adafruit_NeoPixel.h>
#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
//pin for led strip
#define PIN        5 

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 144
//initialises led strip passing the amount of leds what pin it uses and the type of led strip in this case grbw
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

#define DELAYVAL 100 // Time (in milliseconds) to pause between pixels
int strip[144];
int pattern[] = {1,2,3,4,3,1,2,4,2,1,3,4,1,4,1,1,1,1,1,1,1};
int count;
int buffer;
int timer;
int flag;
bool score = false;
void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(9600);
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  mpu.update(); //update gyro
  if(timer>10){ //activates every 10 loops
  timer = 0;
  //sets buffer to next pattern values
  buffer = pattern[count];
  count++;
  //loops count once reached end of pattern array
  if (count == sizeof(pattern) / sizeof(pattern[0]))
  {
    count = 0;
  }
  }
  else{
    //increases loop count
    timer++;
  }
 
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  //loops down the led strip moving each strip values one down
  for(int x=143; x>=0; x--){
    //checks if at the end of strip
    if(x != 0)
    { //if not passes down colour
      strip[x] = strip[x-1];
    }
    else
    { //if at the end checks if buffer has been given a value
      if(buffer == 0)
      { //if not leaves the postion blank
        strip[x] = 0;
      }
      else
      { //if it does loads the buffer values into the strip pulsing a new colour
        strip[x] = buffer;
        //resets buffer to wait for next value given
        buffer = 0;
      }
      
    }
    
  }
  //moves through the led strip
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    //checks if a action values has been given and changes the colour accordingly
    switch (strip[i]) {
    case 1:
    pixels.setPixelColor(i, pixels.Color(255, 15, 15, 0));
    break;
    case 2:
    pixels.setPixelColor(i, pixels.Color(0, 255, 255, 0));
    break;
    case 3:
    pixels.setPixelColor(i, pixels.Color(70, 255, 5, 0));
    break;
    case 4:
    pixels.setPixelColor(i, pixels.Color(255, 165, 0, 0));
    break;
    //leaves blank if empty
    case 0:
    pixels.setPixelColor(i, pixels.Color(0, 0, 0, 0));
    break;
    }
    // pixels.Color() takes RGBW values, from 0,0,0,0 up to 255,255,255,255
    

    

    //pixels.setPixelColor(i, pixels.Color(0, 0, 0, 0));
    
  }
  //sets the area where an action can be played to another colour
  for(int i=0; i<5; i++) { // For each pixel...
  //doesnt overwite an action colour
  if(strip[135-i] == 0){
    pixels.setPixelColor(135-i, pixels.Color(128, 0, 128, 0));
  }
  else
  {
    //sets flag to know what action is in the action area
    flag = strip[135-i];
  }
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
  //tests if an action is in the action area
  if (flag != 0 )
  {
    //test what action is looked for
    switch (flag) {
      //test if the user has played the action at the given time
    case 1:
    //pink up
    if(mpu.getAngleX()>20)
    {
      Serial.print("score");
      flag = 0;
      score = true;
    }
    break;
    case 2:
    //cyan down
    if(mpu.getAngleX()< - 20)
    {
      Serial.print("score");
      flag = 0;
      score = true;
    }
    break;
    case 3:
    //green left
    if(mpu.getAngleY()<-30)
    {
      Serial.print("score");
      flag = 0;
      score = true;
    }
    break;
    //yellow right
    case 4:

    if(mpu.getAngleY()> 30)
    {
      Serial.print("score");
      flag = 0;
      score = true;
    }
    break;
  }
  }
  //gives user feedback on a correct action by changing the colour
  if (score ==true)
  {
    for(int i=0; i<5; i++) { // For each pixel...
    if(strip[135-i] == 0){
    pixels.setPixelColor(135-i, pixels.Color(0, 0, 255, 0));
    }
    }
    pixels.show();
  }
  //resets the score bool for next correct action
  score = false;
  flag = 0;
  Serial.print(mpu.getAngleY());
  Serial.print(mpu.getAngleX());
  Serial.print("\n");
  delay(DELAYVAL); // Pause before next pass through loop
  	
}