#include <Adafruit_NeoPixel.h>
#include <WS2812FX.h>

#include "core.h"

extern Adafruit_NeoPixel pixels;
extern WS2812FX ws2812fx;
extern void all_leds_off();
extern byte btncounter;

void LED_Random(){
    while (btncounter == 0)
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

void LED_Flashlight(){
  while (btncounter == 0)
  {
    for(int i=0;i<NUMPIXELS;i++){
      //pixels.setPixelColor(i, pixels.Color(255,255,255)); 
      pixels.setPixelColor(i, pixels.Color(63,63,63)); 
      pixels.show(); 
    }
  } 
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
  ws2812fx.setMode(FX_MODE_RANDOM_COLOR);
  ws2812fx.start();

  int favs[] = { 7, 40, 43, 10, 16, 8, 14, 19, 41 };
  int favs_current = 0;
  int favs_cnt = 9;
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

extern int ledmode;
void LED_WS2812FX(){
  ws2812fx.setMode(ledmode);
  ws2812fx.start();
  while (btncounter == 0)
  {
    ws2812fx.service();
    delay(10);
  }
}

