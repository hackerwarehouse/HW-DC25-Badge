#include <EEPROM.h>

#include <Adafruit_NeoPixel.h>
#include <WS2812FX.h>
#include <SSD_13XX.h>

extern Adafruit_NeoPixel pixels;
extern WS2812FX ws2812fx;
extern SSD_13XX mydisp;
extern void all_leds_off();
extern byte mydispbrightness;

#include "core.h"

void SaveBrightness(byte a, byte b){  
  pixels.setBrightness(a); 
  ws2812fx.setBrightness(a);
  mydisp.setBrightness(b);
  EEPROM.write(PIXELBRIGHT_ADDR,a);
  EEPROM.write(WS2812FXBRIGHT_ADDR,a);
  EEPROM.write(MYDISPBRIGHT_ADDR,b);
  EEPROM.commit();
  all_leds_off();
  mydispbrightness = b;
}

