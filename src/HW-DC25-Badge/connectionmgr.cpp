#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SSD_13XX.h>
#include <WiFiManager.h>

#include "core.h"

extern SSD_13XX mydisp;
extern WiFiManager wifiManager;
extern byte btncounter;

void ConnectionManager(){
  wifiManager.setAPStaticIPConfig(IPAddress(10,0,5,1), IPAddress(10,0,5,1), IPAddress(255,255,255,0));
  wifiManager.setTimeout(180);
  
  if(!wifiManager.autoConnect("HW-DC25 AP")) 
    {
    Serial.println("failed to connect and hit timeout");
    }
}

