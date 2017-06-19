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

void SysInfo()
{
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  
  mydisp.println("HW-DC25 Badge");
  mydisp.println("  ver 1.0");
  mydisp.println("  rel date 2017-07-01");
  mydisp.println("  src on github.com");
  mydisp.println("");

}

void HWInfo()
{
  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(GREENYELLOW);
  mydisp.setTextScale(1);
  mydisp.println(ESP.getFlashChipRealSize());
  mydisp.println("");
  mydisp.println("");
  mydisp.println("");
  mydisp.println("");
}


