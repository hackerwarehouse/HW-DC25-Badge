#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <SSD_13XX.h>

#include "_images/hw_sm.c"
#include "_images/js_sm.c"
#include "core.h"

extern SSD_13XX mydisp;

void Credits()
{
  mydisp.clearScreen();
  mydisp.setRotation(0);
  mydisp.setTextColor(GREENYELLOW);
  int h,w,buffidx,row,col;
  h = 16;
  w = 16;
  buffidx = 0;
  for (row=0; row<h; row++) {
    for (col=0; col<w; col++) { 
      mydisp.drawPixel(col, row, pgm_read_word(image_data_hw_sm + buffidx));
      buffidx++;
    } 
  }
  mydisp.setCursor(17, 4);
  mydisp.print("Hacker Warehouse");
  mydisp.setCursor(1, 20);
  mydisp.print("hackerwarehouse.com");

  mydisp.setCursor(1, 32);
  buffidx = 0;
  for (row=0; row<h; row++) {
    for (col=0; col<w; col++) { 
      mydisp.drawPixel(col, row+32, pgm_read_word(image_data_js_sm + buffidx));
      buffidx++;
    } 
  }
  mydisp.setCursor(17, 36);
  mydisp.print("Jaycon Systems");
  mydisp.setCursor(1, 52);
  mydisp.print("jayconsystems.com");
}

void SysInfo()
{
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  
  mydisp.println(F("HW-DC25 Badge"));
  mydisp.println(F("  ver 1.0"));
  mydisp.println(F("  rel date 2017-07-01"));
  mydisp.println(F("  src on github.com"));
}

void IPConfig(){
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  
  mydisp.println(F("mac addy: "));
  String mac = WiFi.macAddress();
  mydisp.println("  " + mac);
  mydisp.println(F("Conn status: "));
  mydisp.print(F("  "));
  mydisp.println(WiFi.status());
  mydisp.println(F("IP addy: "));
  mydisp.print(F("  "));
  mydisp.println(WiFi.localIP());
}

void DebugInfo()
{
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  mydisp.println(ESP.getFlashChipSize());
  byte value = EEPROM.read(REGION_ADDR);
  mydisp.println(value);
  value = EEPROM.read(PIXELBRIGHT_ADDR);
  mydisp.println(value);
  value = EEPROM.read(MYDISPBRIGHT_ADDR);
  mydisp.println(value);
  value = EEPROM.read(ARTWORK_ADDR);
  mydisp.println(value);
  value = EEPROM.read(REGION_UNLOCK_ADDR);
  mydisp.println(value);
}


