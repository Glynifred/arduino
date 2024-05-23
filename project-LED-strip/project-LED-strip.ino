// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        5 

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 144

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

#define DELAYVAL 100 // Time (in milliseconds) to pause between pixels
int strip[144];
int pattern[] = {1,2,3,4,3,1,2,4,2,1,3,4,1,4,1,1,1,1,1,1,1};
int count;
int buffer;
int timer;
void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(9600);
}

void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  //mpu.update(); //update gyro
  if(timer>10){ // print data every 10ms
  timer = 0;
  buffer = pattern[count];
  count++;
  if (count == sizeof(pattern) / sizeof(pattern[0]))
  {
    count = 0;
  }
  }
  else{
    timer++;
  }
 
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int x=143; x>=0; x--){
    if(x != 0)
    {
      strip[x] = strip[x-1];
    }
    else
    {
      if(buffer == 0)
      {
        strip[x] = 0;
      }
      else
      {
        strip[x] = buffer;
        buffer = 0;
      }
      
    }
    
  }

  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
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
    case 0:
    pixels.setPixelColor(i, pixels.Color(0, 0, 0, 0));
    break;
    }
    // pixels.Color() takes RGBW values, from 0,0,0,0 up to 255,255,255,255
    

    

    //pixels.setPixelColor(i, pixels.Color(0, 0, 0, 0));
    
  }
  for(int i=0; i<5; i++) { // For each pixel...
  if(strip[135-i] == 0){
    pixels.setPixelColor(135-i, pixels.Color(128, 0, 128, 0));
  }
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(DELAYVAL); // Pause before next pass through loop
  	
}