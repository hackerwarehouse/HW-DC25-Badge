#include <ESP8266WiFi.h>
#include <SSD_13XX.h>
#include <WiFiClient.h>

#include "core.h"

extern SSD_13XX mydisp;

//AP Scanner
void AP_Scanner(){
  int n = WiFi.scanNetworks();
  mydisp.clearScreen();
  for (int i=0; i<n; i++){
    int x = random(0,65535);
    String Name = WiFi.SSID(i);
    mydisp.setTextColor(x);
    mydisp.println(Name);
  }
}
