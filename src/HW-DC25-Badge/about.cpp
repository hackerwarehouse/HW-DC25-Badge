#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <SSD_13XX.h>

#include "_images/hw_sm.c"
#include "_images/js_sm.c"
#include "core.h"

extern SSD_13XX mydisp;
extern volatile byte btnid;
extern byte appmode;
extern byte btncounter;

void SysInfo()
{
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  
  mydisp.println(F("HW-DC25 Badge"));
  mydisp.println(F("  ver 1.3"));
  mydisp.println(F("  rel date 2017-07-05"));
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

void Shouts() {
  appmode=1;
  byte count=1;
  byte last=0;
  btnid = 0;
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  while (1)
  {
    if (btnid == 3){ 
      if (count == 1) {count = 6; } 
      else {count--;}
      btnid = 0;
    }
    else if (btnid == 2){ 
      if (count == 6) {count = 1; } 
      else {count++;}
      btnid = 0;
    }
    else if (btnid == 4) {break;}

    if (count != last)
    {
      switch(count)
      {
       case 1:
          mydisp.clearScreen();
          mydisp.setCursor(0, 0);
          mydisp.setTextScale(2);
          mydisp.println(F("Thanks To:"));
          mydisp.setTextScale(1);
          mydisp.println(F("@fg #badgelife"));
          mydisp.println(F("@rj_chap"));
          mydisp.println(F("@securelyfitz"));
          mydisp.println(F("@thedevilsvoice"));
          mydisp.println(F("@bitstr3m"));
          mydisp.println(F("@SageFedora"));
          mydisp.println(F("0xEnder"));
          break;
       case 2:
          mydisp.clearScreen();
          mydisp.setCursor(0, 0);
          mydisp.println(F("AI4QR"));
          mydisp.println(F("Andronicus"));
          mydisp.println(F("ariore"));
          mydisp.println(F("ArtflD0dg3r"));
          mydisp.println(F("B@nkbr3@k3r"));
          mydisp.println(F("BDAWG"));
          mydisp.println(F("cryptoishard"));
          mydisp.println(F("Csp3r"));
          break;
       case 3:
          mydisp.clearScreen();
          mydisp.setCursor(0, 0);
          mydisp.println(F("d1g1t4l_t3mpl4r"));
          mydisp.println(F("DaKahuna"));
          mydisp.println(F("dal3ksec"));
          mydisp.println(F("Dasbear"));
          mydisp.println(F("donds i.sheep.hunt"));
          mydisp.println(F("Duck Duck -  DC603"));
          mydisp.println(F("fico"));
          mydisp.println(F("HealWHans Hak4Kidz"));
          break;
       case 4:
          mydisp.clearScreen();
          mydisp.setCursor(0, 0);
          mydisp.println(F("Hyr0n ~ AND!XOR"));
          mydisp.println(F("Isiah"));
          mydisp.println(F("jthoel"));
          mydisp.println(F("Kit Kitsune"));
          mydisp.println(F("mauvehed"));
          mydisp.println(F("mediumrehr"));
          mydisp.println(F("monolith"));
          mydisp.println(F("Phaded"));
          break;
       case 5:
          mydisp.clearScreen();
          mydisp.setCursor(0, 0);
          mydisp.println(F("r0d3nt - irc 2600"));
          mydisp.println(F("Ridgeback"));
          mydisp.println(F("TechGirlMN"));
          mydisp.println(F("Thistledown"));
          mydisp.println(F("TibitXimer"));
          mydisp.println(F("Trippbit"));
          mydisp.println(F("tusuzu"));
          mydisp.println(F("Wasabi"));
          break;
       case 6:
          mydisp.clearScreen();
          mydisp.setCursor(0, 0);
          mydisp.println(F("webbreacher"));
          mydisp.println(F("WillyCrash"));
          mydisp.println(F("wishbone"));
          mydisp.println(F("Zapp"));
          mydisp.println(F("zarak"));
          mydisp.println(F("Zq"));
          mydisp.println(F("...and many other"));
          mydisp.println(F("unnamed supporters"));
          break;
      }
      last=count;
    }
    delay(100);
  }
  appmode=0;
  btncounter++;
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


