#include <SSD_13XX.h>
#include <SPI.h>

#include "core.h"

extern SSD_13XX mydisp;
extern volatile byte btnid;
extern byte appmode;
extern byte btncounter;

void Shouts1() {  
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
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
}

void Shouts2() {
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println(F("AI4QR"));
  mydisp.println(F("Andronicus"));
  mydisp.println(F("ariore"));
  mydisp.println(F("ArtflD0dg3r"));
  mydisp.println(F("B@nkbr3@k3r"));
  mydisp.println(F("BDAWG"));
  mydisp.println(F("cryptoishard"));
  mydisp.println(F("Csp3r"));
}

void Shouts3() {
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println(F("d1g1t4l_t3mpl4r"));
  mydisp.println(F("DaKahuna"));
  mydisp.println(F("dal3ksec"));
  mydisp.println(F("Dasbear"));
  mydisp.println(F("donds i.sheep.hunt"));
  mydisp.println(F("Duck Duck -  DC603"));
  mydisp.println(F("fico"));
  mydisp.println(F("HealWHans Hak4Kidz"));
}

void Shouts4() {
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println(F("Hyr0n ~ AND!XOR"));
  mydisp.println(F("Isiah"));
  mydisp.println(F("jthoel"));
  mydisp.println(F("Kit Kitsune"));
  mydisp.println(F("mauvehed"));
  mydisp.println(F("mediumrehr"));
  mydisp.println(F("monolith"));
  mydisp.println(F("Phaded"));
}

void Shouts5() {
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println(F("r0d3nt - irc 2600"));
  mydisp.println(F("Ridgeback"));
  mydisp.println(F("TechGirlMN"));
  mydisp.println(F("Thistledown"));
  mydisp.println(F("TibitXimer"));
  mydisp.println(F("Trippbit"));
  mydisp.println(F("tusuzu"));
  mydisp.println(F("Wasabi"));
}

void Shouts6() {
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println(F("webbreacher"));
  mydisp.println(F("WillyCrash"));
  mydisp.println(F("wishbone"));
  mydisp.println(F("Zapp"));
  mydisp.println(F("zarak"));
  mydisp.println(F("Zq"));
  mydisp.println(F("...and many other"));
  mydisp.println(F("unnamed supporters"));
}

void Shouts() {
  appmode=1;
  byte count=1;
  byte last=0;
  btnid = 0;
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
          Shouts1();
          //last=1;
          break;
       case 2:
          Shouts2();
          //last=2;
          break;
       case 3:
          Shouts3();
          //last=3;
          break;
       case 4:
          Shouts4();
          //last=4;
          break;
       case 5:
          Shouts5();
          //last=5;
          break;
       case 6:
          Shouts6();
          //last=6;
          break;
      }
      last=count;
    }
    delay(100);
  }
  appmode=0;
  btncounter++;
}
