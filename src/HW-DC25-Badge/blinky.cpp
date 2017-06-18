#include <Adafruit_NeoPixel.h>

#include "core.h"

extern Adafruit_NeoPixel pixels;
extern void all_leds_off();
extern byte counter;

//Random Blinking 
void LED_Random(){
    while (counter <= 0)
    {
      for(int i=0;i<NUMPIXELS;i++)
      {
        int x = random(0,255); 
        int y = random(0,255);
        int z = random(0,255);
        pixels.setPixelColor(i, pixels.Color(x,y,z)); 
        pixels.show(); 
      }
      delay(1000);
    }
}

//Larson Scanner
void LED_Larson(){
  int pos = 0, dir = 1;
  int j;

  while (counter <= 0)
  {
    pixels.setPixelColor(pos - 1, 0xFF3000); // Medium red
    pixels.setPixelColor(pos    , 0x800000); // Center pixel is brightest
    pixels.setPixelColor(pos + 1, 0xFF3000 ); // Medium red
    pixels.show();
    delay(50);
   
    for(j=-2; j<= 2; j++) pixels.setPixelColor(pos+j, 0);
   
    pos += dir;
    if(pos < 0) {
      pos = 1;
      dir = -dir;
    } else if(pos >= pixels.numPixels()) {
      pos = pixels.numPixels() - 2;
      dir = -dir;
    }
  }
}

//LED Chase
void LED_Chase(){
  while (counter <= 0)
  {
    int del = 100;
    for (int i=0; i<NUMPIXELS; i++){
      if(counter > 0){
        break;
      }
      else if(i==0){
        pixels.setPixelColor(i, pixels.Color(random(0,255),random(0,255),random(0,255)));
      }
      else{
        pixels.setPixelColor(i, pixels.Color(random(0,255),random(0,255),random(0,255)));
        pixels.setPixelColor(i-1, pixels.Color(0,0,0));      
      }
      pixels.show();
      delay(del);
    }
    pixels.setPixelColor(13, pixels.Color(0,0,0));
    pixels.show();
    delay(del);
  
    for (int i=13; i>-1; i--){
      if(counter > 0){
        break;
      }
      else if (i==13){
        pixels.setPixelColor(i, pixels.Color(random(0,255),random(0,255),random(0,255)));
      }
      else{
        pixels.setPixelColor(i, pixels.Color(random(0,255),random(0,255),random(0,255)));
        pixels.setPixelColor(i+1, pixels.Color(0,0,0));
      }
      pixels.show();
      delay(del);
    }
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.show();
    delay(del);
  }
}

//Flashlight 
void LED_Flashlight(){
  while (counter <= 0)
  {
    for(int i=0;i<NUMPIXELS;i++){
      //pixels.setPixelColor(i, pixels.Color(255,255,255)); 
      pixels.setPixelColor(i, pixels.Color(63,63,63)); 
      pixels.show(); 
    }
  } 
}
