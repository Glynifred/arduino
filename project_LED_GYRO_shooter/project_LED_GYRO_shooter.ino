//loads libarys of neopixel and gyro
#include <Adafruit_NeoPixel.h>
#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
//pin for led strip
#define PIN        5 
//pin for button
#define btn 2 
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 144

//initialises led strip passing the amount of leds what pin it uses and the type of led strip in this case grbw
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);
#define DELAYVAL 32 //time betwen each loop aka framerate of 30 per second about
//strip used to map onto LED strip
bool strip[144];
//level used to hold the full level array to then map onto strip
bool level[576];
//count used as a reusable counter
int count;
//remainder to loop strip after its gone past end of the level
int rem;
//flag to stop multiple shots
bool flag;
//pattern used to propergate level
int pattern[] = {7,4,6,2,5,3,2,4,5,1,2,3,4,5,6,2,3,4,5,1,2,3,4,5,6,2,3,4,10};
//tracks amount of enemies
int enemies;
int position = 0;//will dictate what location the led strip is showing in the array level
//rgb values used for displaying enemies remaining
int red = 10;
int blue = 1;
int green = 1;
//tracks amount of bullets left
int bullets = 3;
//function activated on start
void setup() {
 pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(9600);//starts serial outputs used for checking everything is working
  //starts gyro
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); //starts gyro making sure values start at 0
  Serial.println("Done!\n");
  //sets up button without a resitor
  pinMode(btn, INPUT_PULLUP);
  //propergates level based on pattern where each value us the distance between the enemies and the size of array is the amount of enemies in the level
  for (int i=0; i<(sizeof(pattern) / sizeof(pattern[0])); i++)//loops for the amount of positions in pattern array
  {
    //activates 4 times to make the enemies 4 pixles wide
    for (int x=0; x<pattern[i]*4; x++)
    {
      level[count] = 0;
      count++;
      

    }
    for (int x=0; x<4; x++)
    {
      level[count] = 1;
      count++;
    }  
  }
  enemies = sizeof(pattern) / sizeof(pattern[0]);//sets enemies to size of pattern array
  Serial.print(enemies);
  count = 0;//resets count
}
//loops every frame
void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  

  for(int i=0; i<NUMPIXELS; i++) { //actiavtes for each pixel
  //checks if the led has passed level and needs to wrap
  if (i + position > 575)
  {
    strip[i] = level[i - rem];
  }
  else
  {
    strip[i] = level[i + position];
  } //checks what values should the led represent
      switch (strip[i]) {
      case 1:
      //sets led to an enemy
      pixels.setPixelColor(i, pixels.Color(10, 0, 0));
      break;
      case 0:
        //triggers background creation based on different conditions
        if((i + position) %64 != 25 && (i + position) %64 != 3 && (i + position) %64 < 42 || (i + position) %64 > 45 )
        {
          pixels.setPixelColor(i, pixels.Color(5, 4, 0));
        }
        else if((i + position) %64 != 25 && (i + position) %64 != 3 ) 
        {
          pixels.setPixelColor(i, pixels.Color(3, 2, 1));
        }
        else
        {
          pixels.setPixelColor(i, pixels.Color(1, 2, 1));
        }
      
      break;
      default:
      break;
      }  
  }
  mpu.update(); //update gyro
  
  //shoots if gyro is positioned upwards, the user has bullets and the flag is 0
  if(mpu.getAngleX()>30 && flag == 0 && bullets > 0)
    {
      //sets flag to 1 so multiple shots arent triggered
      flag = 1;
      //checks if an enemy is within bounds of target, 68,72 and 75 are the only points need to 100% confirm that an enemy is located within the targets of 68-75
      if(strip[68] == 1)
      {
        count = 68;
      }
      else if (strip[72] == 1)
      {
        count = 72;
      }
      else if((strip[75] == 1))
      {
        count = 75;
      }
      //checks if an enemy was found
      if (count != 0)
      { //goes 3 either side to make sure enemy is found
        for(int i=0; i<7; i++)
      { //checks 3 to the left and then moves right
        if (strip[count - 3 + i] == 1)
        { //checks if the enemy is warped onto the start, then removes the enemy from the level array
          if (count - 3 + i + position > 575)
          {
          level[count - 3 + i - rem] = 0;
          }
          else
          level[count - 3 + i + position] = 0;
        }
      }
      //reduces the amount of enemies
      enemies--;
      Serial.print(enemies);
      count = 0;
      }
      //reduces the amount of bullets
      bullets--;
      
    }
    //allows user to shoot again only once returned hand to neutral
    if (mpu.getAngleX()<25)
    {
      flag = 0;
    }
    //move left
  if(mpu.getAngleY()<-30 && flag == 0)
    {
      position--;
      flag = 1;

    }
    //move right
  if(mpu.getAngleY()>30 && flag == 0)
    {
      position++;
      flag = 1;
    }
  //checks if positon has looped fully around level
  if(position >= 576)
  {
    position = 0;
  }
  //checks if positon has looped backwards
  if ( position < 0)
  {
    position = 575;
  }
  //checks if part of the led strip loops around
  if(position + 144 >= 575)
  {
    //sets remainder to loop the led strip
    rem = 144 - (position + 144 - 575);
  }
  //sets the traget leds
  pixels.setPixelColor(68, pixels.Color(50, 3, 50, 0));
  pixels.setPixelColor(75, pixels.Color(50, 3, 50, 0));
  //checks if the enemies remaining is a power of 10 but not 0 to display how many remain
  if (enemies%10 == 0 && enemies != 0)
  {
     for(int i=0; i<10; i++)//loops 10 times
  {   //changes rgb values based on the power of ten
      switch ((enemies - 1)/10) {
      case 0:
      red = 10;
      blue = 1;
      green = 1;
      break;
      case 1:
      red = 1;
      blue = 10;
      green = 1;
      break;
      case 2:
      red = 1;
      blue = 1;
      green = 10;
      break;
      }
      //sets pixels to resssemble remaining enemy count
    pixels.setPixelColor(NUMPIXELS - 1 -(i *2), pixels.Color(red,green,blue));
  }
  }
  //if not ten works the same but needed ten to show ten dots instead of none
  else
  {
    for(int i=0; i<enemies%10; i++)
  {
      switch (enemies/10) {
      case 0:
      red = 10;
      blue = 1;
      green = 1;
      break;
      case 1:
      red = 1;
      blue = 10;
      green = 1;
      break;
      case 2:
      red = 1;
      blue = 1;
      green = 10;
      break;
      }
    pixels.setPixelColor(NUMPIXELS - 1 -(i *2), pixels.Color(red,green,blue));
  }
  }
  //displays how many bullets the user has
   for(int i=0; i<bullets; i++)
  {
     
    pixels.setPixelColor(0 + (i *2), pixels.Color(148,0,211));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
  //Serial.print(mpu.getAngleY());
  //Serial.print(mpu.getAngleX());
  //Serial.print("\n");
  //if button pressed
  if (digitalRead(btn) == 0) {
    bullets = 3;
  }
  delay(DELAYVAL); // Pause before next pass through loop
  	
}
