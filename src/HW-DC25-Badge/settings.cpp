#include <EEPROM.h>

#include <Adafruit_NeoPixel.h>
#include <WS2812FX.h>
#include <SSD_13XX.h>

#include "_images/badge.c"
#include "_images/godai_1.c"
#include "_images/defcon.c"

extern Adafruit_NeoPixel pixels;
extern WS2812FX ws2812fx;
extern SSD_13XX mydisp;
extern void all_leds_off();
extern byte mydispbrightness;
extern byte appmode;
extern volatile byte id;
extern byte btncounter;

#include "core.h"

void DisplayArtwork(byte img){
  int h, w, buffidx;
  switch(img)
  {
   case 1:
      h = 63;
      w = 73;
      buffidx = 0;
      mydisp.clearScreen();
      for (int row=0; row<h; row++) {
        for (int col=0; col<w; col++) { 
          mydisp.drawPixel(col+11, row, pgm_read_word(image_data_badge + buffidx));
          buffidx++;
        } 
      }
      break;
   case 2:
      h = 63;
      w = 63;
      buffidx = 0;
      mydisp.clearScreen();
      //mydisp.drawImage(17,0,&godai_1);
      for (int row=0; row<h; row++) {
        for (int col=0; col<w; col++) { 
          mydisp.drawPixel(col+17, row, pgm_read_word(image_data_godai_1 + buffidx));
          buffidx++;
        } 
      }
      break;
   case 3:
      h = 25;
      w = 96;
      buffidx = 0;
      mydisp.clearScreen(); 
      //mydisp.drawImage(0,20,&defcon);
      for (int row=0; row<h; row++) {
        for (int col=0; col<w; col++) { 
          mydisp.drawPixel(col, row+20, pgm_read_word(image_data_defcon + buffidx));
          buffidx++;
        } 
      }
      break;
  }
}

void SelectArtwork(){
  appmode=1;
  int count=1;
  int last=0;
  id = 0;
  while (1)
  {
    if (id == 3){ 
      if (count == 1) {count = 3; } 
      else {count--;}
      id = 0;
    }
    else if (id == 2){ 
      if (count == 3) {count = 1; } 
      else {count++;}
      id = 0;
    }
    else if (id == 1){
      EEPROM.write(ARTWORK_ADDR,count);
      EEPROM.commit();
      mydisp.clearScreen();
      mydisp.setCursor(0, 0);
      mydisp.setTextColor(GREENYELLOW);
      mydisp.setTextScale(3);
      mydisp.println(F("Saved"));
      delay(1000);
      id = 4;
      break;
    }
    else if (id == 4) {break;}

    if (count != last)
    {
      switch(count)
      {
       case 1:
          last=1;
          DisplayArtwork(last);
          break;
       case 2:
          last=2;
          DisplayArtwork(last);
          break;
       case 3:
          last=3;
          DisplayArtwork(last);
          break;
      }
    }
    delay(100);
  }
  appmode=0;
  btncounter++;
}

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

