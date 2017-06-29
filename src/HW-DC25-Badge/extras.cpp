#include <EEPROM.h>

#include <SSD_13XX.h>

extern SSD_13XX mydisp;
extern byte appmode;
extern byte btncounter;
extern volatile byte btnid;

extern byte region_unlocked;

#include "core.h"
#include "textinput.h"

#define QUEUE_SIZE 8

void ButtonEcho(){
  appmode=1;
  btnid = 0;
  byte chars=1;
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
        
        for (int i = 0; i < (sizeof(UNLOCK_KONAMI)/sizeof(UNLOCK_KONAMI[0]) - 1); i++) { b += char(pgm_read_byte_near(UNLOCK_KONAMI + i)); }
        if (a == b) { 
          mydisp.print(F("konami!")); 
          chars+=7;
        };
        b = "";

        for (int i = 0; i < (sizeof(UNLOCK_REGION)/sizeof(UNLOCK_REGION[0]) - 1); i++) { b += char(pgm_read_byte_near(UNLOCK_REGION + i)); }
        if (a == b) { 
          mydisp.print(F("region unlocked")); 
          region_unlocked = 1;
          delay(2000);
          break;
        };
        b = "";

        for (int i = 0; i < (sizeof(UNLOCK_REGIONPERM)/sizeof(UNLOCK_REGIONPERM[0]) - 1); i++) { b += char(pgm_read_byte_near(UNLOCK_REGIONPERM + i)); }
        if (a == b) { 
          mydisp.print(F("region perm unlocked"));
          EEPROM.write(REGION_UNLOCK_ADDR,1);
          EEPROM.commit(); 
          region_unlocked = 1;
          delay(2000);
          break;
        };
        b = "";
        
        a="";
      }
      if (chars >= 136) { mydisp.clearScreen(); chars=0;}
    }
    delay(100);
  }
  appmode=0;
  btnid = 4;
  btncounter++;
}

void KeyboardDemo(){
  appmode=1;
  btnid = 0;
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  String t = "";
  TextInput(F("Input text via d-pad: "), t, 8);
  if (t) { mydisp.print(t); }
  delay(2000);
  appmode=0;
  btnid = 4;
  btncounter++;
}

