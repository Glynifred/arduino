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

#define DELAYVAL 10 // Time (in milliseconds) to pause between pixels
void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
   Serial.begin(9600);
  Wire.begin();
  
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

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    mpu.update();
    // pixels.Color() takes RGBW values, from 0,0,0,0 up to 255,255,255,255
    pixels.setPixelColor(i, pixels.Color(map(mpu.getAngleX(),0,90,0,255), map(mpu.getAngleY(),0,90,0,255), map(mpu.getAngleZ(),0,90,0,255), 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }

    for(int i=NUMPIXELS-1; i>=0; i--) { // Go back the other way...

    pixels.show();   // Send the updated pixel colors to the hardware.

    pixels.setPixelColor(i, pixels.Color(0, 0, 0, 0));

    delay(DELAYVAL); // Pause before next pass through loop
  }
}
