#include <ESP8266WiFi.h>
#include <SSD_13XX.h>

#include "_images/hacker.c"
#include "_images/js.c"
#include "core.h"

extern SSD_13XX mydisp;

void Credits()
{
  mydisp.clearScreen();
  mydisp.setRotation(0);
  mydisp.setTextColor(GREENYELLOW);

  mydisp.drawImage(0, 0, &hacker);
  mydisp.setCursor(17, 4);
  mydisp.print("Hacker Warehouse");
  mydisp.setCursor(1, 20);
  mydisp.print("hackerwarehouse.com");
  
  mydisp.drawImage(1, 32, &js);
  mydisp.setCursor(17, 36);
  mydisp.print("Jaycon Systems");
  mydisp.setCursor(1, 52);
  mydisp.print("jayconsystems.com");
}

void Version()
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
  mydisp.println(F(""));
  mydisp.println();
  mydisp.println(F(""));
  mydisp.println(F(""));
}


