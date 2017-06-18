#include <SSD_13XX.h>
#include <SPI.h>

#include "core.h"

extern SSD_13XX mydisp;
extern byte counter;

unsigned long Shouts1() {
  mydisp.clearScreen();
  unsigned long start = micros();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(2);
  mydisp.println("Thanks To:");
  mydisp.setTextScale(1);
  mydisp.println("@fg #badgelife");
  mydisp.println("@rj_chap");
  mydisp.println("@securelyfitz");
  mydisp.println("@thedevilsvoice");
  mydisp.println("@bitstr3m");
  mydisp.println("@SageFedora");
  mydisp.println("0xEnder");
  return micros() - start;
}

unsigned long Shouts2() {
  mydisp.clearScreen();
  unsigned long start = micros();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println("AI4QR");
  mydisp.println("Andronicus");
  mydisp.println("ArtflD0dg3r");
  mydisp.println("B@nkbr3@k3r");
  mydisp.println("BDAWG");
  mydisp.println("cryptoishard");
  mydisp.println("Csp3r");
  mydisp.println("d1g1t4l_t3mpl4r");
  return micros() - start;
}

unsigned long Shouts3() {
  mydisp.clearScreen();
  unsigned long start = micros();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println("DaKahuna");
  mydisp.println("dal3ksec");
  mydisp.println("Dasbear");
  mydisp.println("donds i.sheep.hunt");
  mydisp.println("Duck Duck -  DC603");
  mydisp.println("HealWHans Hak4Kidz");
  mydisp.println("Hyr0n ~ AND!XOR");
  mydisp.println("Isiah");
  return micros() - start;
}

unsigned long Shouts4() {
  mydisp.clearScreen();
  unsigned long start = micros();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println("jthoel");
  mydisp.println("Kit Kitsune");
  mydisp.println("mauvehed");
  mydisp.println("mediumrehr");
  mydisp.println("monolith");
  mydisp.println("Phaded");
  mydisp.println("r0d3nt - irc 2600");
  mydisp.println("Ridgeback");
  return micros() - start;
}

unsigned long Shouts5() {
  mydisp.clearScreen();
  unsigned long start = micros();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println("TechGirlMN");
  mydisp.println("Thistledown");
  mydisp.println("TibitXimer");
  mydisp.println("Trippbit");
  mydisp.println("tusuzu");
  mydisp.println("Wasabi");
  mydisp.println("webbreacher");
  mydisp.println("WillyCrash");
  return micros() - start;
}

unsigned long Shouts6() {
  mydisp.clearScreen();
  unsigned long start = micros();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println("wishbone");
  mydisp.println("Zapp");
  mydisp.println("zarak");
  mydisp.println("Zq");
  mydisp.println("");
  mydisp.println("and many other early");
  mydisp.println("supporters");
  mydisp.println("");
  return micros() - start;
}

void Shouts() {
  while (counter <= 0)
  {
    Serial.println(Shouts1());
    delay(3000);
    Serial.println(Shouts2());
    delay(3000);
    Serial.println(Shouts3());
    delay(3000);
    Serial.println(Shouts4());
    delay(3000);
    Serial.println(Shouts5());
    delay(3000);
    Serial.println(Shouts6());
    delay(3000);
  }
}
