#include <EEPROM.h>

#include <Adafruit_NeoPixel.h>
#include <WS2812FX.h>
#include <SSD_13XX.h>

extern void all_leds_off();
extern Adafruit_NeoPixel pixels;
extern WS2812FX ws2812fx;
extern SSD_13XX mydisp;
extern volatile byte btnid;
extern byte appmode;
extern byte btncounter;
extern byte mydispbrightness;
extern byte region_id;
extern byte region_unlocked;

#include "core.h"

void loadSettings(){
  EEPROM.begin(512);  //Set eeprom - 512 bytes
  byte value=0;

  region_id = EEPROM.read(REGION_ADDR);
  if (region_id == 255){
    region_id = 1;                          //default 1 -> US
    EEPROM.write(REGION_ADDR,region_id);
    EEPROM.commit();
  }

  value = EEPROM.read(PIXELBRIGHT_ADDR);
  if (value == 255){
    value = 64;                             //default 64 ~ 1/4 brightness overall
    EEPROM.write(PIXELBRIGHT_ADDR,value);
    EEPROM.commit();
  }
  pixels.setBrightness(value);

  mydispbrightness = EEPROM.read(MYDISPBRIGHT_ADDR);
  if (mydispbrightness == 255){
    value = 4;                              //default 4 ~ 1/4 brightness overall
    EEPROM.write(MYDISPBRIGHT_ADDR,mydispbrightness);  
    EEPROM.commit();
  }
  mydisp.setBrightness(mydispbrightness);

  value = EEPROM.read(WS2812FXBRIGHT_ADDR);
  if (value == 255){
    value = 64;                              //default 64 ~ 1/4 brightness overall
    EEPROM.write(WS2812FXBRIGHT_ADDR,value); 
    EEPROM.commit();
  }
  ws2812fx.setBrightness(value);

  region_unlocked = EEPROM.read(REGION_UNLOCK_ADDR);
  if (region_unlocked == 255){
    region_unlocked = 0;
    EEPROM.write(REGION_UNLOCK_ADDR,region_unlocked); 
    EEPROM.commit();
  }  
}

void SaveBrightness(byte a, byte b){  
  pixels.setBrightness(a); 
  ws2812fx.setBrightness(a);
  mydisp.setBrightness(b);
  EEPROM.write(PIXELBRIGHT_ADDR,a);
  EEPROM.write(WS2812FXBRIGHT_ADDR,a);
  EEPROM.write(MYDISPBRIGHT_ADDR,b);
  EEPROM.commit();
  all_leds_off();
  mydispbrightness = b;
}

///////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

void SettingsViaWiFi() {
  appmode=1;
  byte count=1;
  byte last=0;
  btnid = 0;
 
  String ssid = "HW-";
  ssid += random(0,255);
  ssid += random(0,99);
  String password = "badge";
  password += random(0,255);
  password += random(0,99);
  password += random(0,255);

  mydisp.clearScreen();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println(F("Connect to..."));
  mydisp.print(F("ssid: "));
  mydisp.println(ssid);
  mydisp.print(F("password: "));
  mydisp.println(password);
  mydisp.println();
  mydisp.println(F("then browse to"));
  mydisp.println(F("  10.0.5.1"));

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(10,0,5,1), IPAddress(10,0,5,1), IPAddress(255,255,255,0));
  WiFi.softAP(ssid.c_str(), password.c_str(), 9, 0);
  
  while (1)
  {
    if (btnid == 4) {break;}
    delay(100);
  }

  WiFi.softAPdisconnect(1);
  appmode=0;
  btncounter++;
}


