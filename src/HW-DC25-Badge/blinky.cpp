#include <EEPROM.h>

#include <Adafruit_NeoPixel.h>
#include <WS2812FX.h>

#include "core.h"

extern Adafruit_NeoPixel pixels;
extern WS2812FX ws2812fx;
extern void all_leds_off();
extern byte btncounter;
extern volatile byte btnid;
extern byte appmode;

void LED_Flashlight(){
  appmode=1;
  byte count=3;
  byte last=0;
  btnid = 0;
  while (1)
  {
    if (btnid == 3){ 
      if (count == 1) {count = 1; } 
      else {count--;}
      btnid = 0;
    }
    else if (btnid == 2){ 
      if (count == 5) {count = 5; } 
      else {count++;}
      btnid = 0;
    }
    else if (btnid == 4) {break;}

    if (count != last)
    {
      switch(count)
      {
       case 1:
          pixels.setBrightness(255);
          last=1;
          break;
       case 2:
          pixels.setBrightness(184);
          last=2;
          break;
       case 3:
          pixels.setBrightness(128);
          last=3;
          break;
       case 4:
          pixels.setBrightness(64);
          last=4;
          break;
       case 5:
          pixels.setBrightness(10);
          last=5;
          break;
      }
    }
    for(byte i=0;i<NUMPIXELS;i++){
      //pixels.setPixelColor(i, pixels.Color(255,255,255)); 
      pixels.setPixelColor(i, pixels.Color(63,63,63)); 
      pixels.show(); 
    }
    delay(100);
  }
  appmode=0;
  btncounter++;
  pixels.setBrightness(EEPROM.read(PIXELBRIGHT_ADDR));
}

#define TIMER_MS 7000
void LED_WS2812FX_FullDemo(){
  unsigned long last_change = 0;
  unsigned long now = 0;
  ws2812fx.setMode(FX_MODE_RANDOM_COLOR);
  ws2812fx.start();

  while (btncounter == 0)
    {
      now = millis();
      ws2812fx.service();
      if(now - last_change > TIMER_MS) {
        ws2812fx.setMode((ws2812fx.getMode() + 1) % ws2812fx.getModeCount());
        last_change = now;
      }
      delay(10);
    }
}

void LED_WS2812FX_Favorites(){
  unsigned long last_change = 0;
  unsigned long now = 0;
  ws2812fx.setMode(FX_MODE_TWINKLE_RANDOM);
  ws2812fx.start();

  byte favs[] = { 7, 40, 43, 10, 17, 8, 14, 19, 41 };
  byte favs_cnt = 8;
  byte favs_current = 0;
  while (btncounter == 0)
    {
      now = millis();
      ws2812fx.service();
      if(now - last_change > TIMER_MS) {
        ws2812fx.setMode(favs[favs_current]);
        if (favs_current == favs_cnt) { favs_current = 0; }
        else { favs_current = favs_current + 1; }
        last_change = now;
      }
      delay(10);
    }
}

void LED_WS2812FX(byte ledmode){
  ws2812fx.setMode(ledmode);
  ws2812fx.start();
  while (btncounter == 0)
  {
    ws2812fx.service();
    delay(10);
  }
}

