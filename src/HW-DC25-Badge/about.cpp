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
  mydisp.setRotation(0);
  mydisp.setTextColor(GREENYELLOW);

  mydisp.setCursor(1, 4);
  mydisp.print("HW-DC25 Badge");
  mydisp.setCursor(1, 20);
  mydisp.print(" ver 1.0");
  mydisp.setCursor(1, 36);
  mydisp.print(" rel date 2017-07-01");
  mydisp.setCursor(1, 52);
  mydisp.print(" src on github.com");
}


