#include <EEPROM.h>

#include <SSD_13XX.h>

extern SSD_13XX mydisp;
extern byte appmode;
extern byte btncounter;
extern volatile byte btnid;

#include "core.h"

#define QUEUE_SIZE 8

void ButtonEcho(){
  appmode=1;
  byte chars=1;
  btnid = 0;
  String a = "";

  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  
  while (1)
  {
    if (btnid == 0){}
    else
    {
      if (btnid == 3){ 
        mydisp.print(F("U"));
        a += 'U';
        chars++;
        btnid = 0;
      }
      else if (btnid == 2){ 
        mydisp.print(F("D"));
        a += 'D';
        chars++;
        btnid = 0;
      }
      else if (btnid == 1){
        mydisp.print(F("R"));
        a += 'R';
        chars++;
        btnid = 0;
      }
      else if (btnid == 4) {
        mydisp.print(F("L"));
        a += 'L';
        chars++;
        btnid = 0;
      }
      if (a.length() == QUEUE_SIZE)
      {
        String b = "";
        
        for (int i = 0; i < (sizeof(UNLOCK_REGION)/sizeof(UNLOCK_REGION[0]) - 1); i++) { b += char(pgm_read_byte_near(UNLOCK_REGION + i)); }
        if (a == b) { 
          mydisp.print(F("unlocked")); 
          b = ""; 
        };

//        region_id = EEPROM.read(REGION_ADDR);
//        if (region_id == 255){
//          EEPROM.write(REGION_ADDR,1);  //default 1 -> US
//          EEPROM.commit();
//          region_id = 1;
//        }
        
        a="";
        
      }
      if (chars == 136) { mydisp.clearScreen(); chars=0;}
    }
    delay(100);
  }
  appmode=0;
  btncounter++;

}

