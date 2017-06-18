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
  
//  mydisp.drawImage(0, 0, &js);
//  mydisp.setCursor(17, 4);
//  mydisp.print("Jaycon Systems");
//  
//  mydisp.drawImage(1, 16, &hacker);
//  mydisp.setCursor(17, 20);
//  mydisp.print("Hacker Warehouse");
//  mydisp.drawImage(1, 32, &godai);
//  mydisp.setCursor(17, 36);
//  mydisp.print("Godai Group");
//  
//  mydisp.drawImage(1, 48, &garret);
//  mydisp.setCursor(17, 52);
//  mydisp.print("Garrett Gee 2");

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
