#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <EEPROM.h>
#include <SPI.h>

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <qMenuDisplay.h>
#include <qMenuSystem.h>
#include <SSD_13XX.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <WS2812FX.h>
#include "_fonts/defaultFont.c"

#include "about.h"
#include "apscanner.h"
#include "benchmark.h"
#include "blinky.h"
#include "channelactivity.h"
#include "connectionmgr.h"
#include "core.h"
#include "mainmenu.h"
#include "shouts.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
WS2812FX ws2812fx = WS2812FX(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
SSD_13XX mydisp(_CS, _DC);
qMenuSystem menu=qMenuSystem(&mydisp);
WiFiManager wifiManager;

const byte left = 15; 
const byte down = 10; 
const byte right = 2; 
const byte up = 0;  

volatile byte btncounter = 0;
volatile byte id  = 0;
byte ledmode = 0;
byte appmode = 0;
volatile byte region_id = 1;

long debouncing_time = 250;
unsigned long last_micros = 0;

void UP(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    if (appmode){      
    }
    else {
      btncounter ++;
    }
    id = 3;
    last_micros = micros();
  }
}

void DOWN(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) 
  {
    if (appmode){
    }
    else {
      btncounter ++;
    }
    id = 2;
    last_micros = micros();
  }
}

void RIGHT(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    if (appmode){
    }
    else {
      btncounter ++;
    }
    id = 1;
    last_micros = micros();
  }
}

void LEFT(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    if (appmode){
    }
    else {
      btncounter ++;
    }
    id = 4;
    last_micros = micros();
  }
}

void setup()
{  
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(left, INPUT);

  attachInterrupt(digitalPinToInterrupt(up), UP, FALLING);
  attachInterrupt(digitalPinToInterrupt(down), DOWN, FALLING);
  attachInterrupt(digitalPinToInterrupt(right), RIGHT, FALLING);
  attachInterrupt(digitalPinToInterrupt(left), LEFT, RISING);

  menu.InitMenu((const char **)mnuRoot,cntRoot,1);

  pixels.begin();
  ws2812fx.init();
  ws2812fx.setSpeed(200);

  loadSettings();
  all_leds_off();
  wifi_off();           // not sure if I should keep this here
  
  Serial.begin(115200);
}

void loadSettings(){
  EEPROM.begin(512);  //Set eeprom - 512 bytes
  byte value=0;

  region_id = EEPROM.read(REGION_ADDR);
  if (region_id == 255){
    EEPROM.write(REGION_ADDR,1);  //default 1 -> US
    EEPROM.commit();
  }

  value = EEPROM.read(PIXELBRIGHT_ADDR);
  if (value == 255){
    value = 64;                             //default 64 ~ 1/4 brightness overall
    EEPROM.write(PIXELBRIGHT_ADDR,value);
    EEPROM.commit();
  }
  pixels.setBrightness(value);

  value = EEPROM.read(MYDISPBRIGHT_ADDR);
  if (value == 255){
    value = 4;                              //default 4 ~ 1/4 brightness overall
    EEPROM.write(MYDISPBRIGHT_ADDR,value);  
    EEPROM.commit();
  }
  mydisp.setBrightness(value);

  value = EEPROM.read(WS2812FXBRIGHT_ADDR);
  if (value == 255){
    value = 64;                              //default 64 ~ 1/4 brightness overall
    EEPROM.write(WS2812FXBRIGHT_ADDR,value); 
    EEPROM.commit();
  }
  ws2812fx.setBrightness(value);
}

void resetMenu(){
  all_leds_off();
  mydisp.setFont(&defaultFont);
}

void all_leds_off(){
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
  }  
}

void wifi_off(){
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
}

void loop()
{ 
  int keycode=0;
  int clickedItem=0; 
  
  if (btncounter > 0)
  { 
    switch(id)
    {
      case 1:       
        clickedItem=menu.ProcessMenu(ACTION_SELECT);
        btncounter--;
        break;
      case 2:
        menu.ProcessMenu(ACTION_DOWN);
        btncounter--;
        break;      
      case 3:
        menu.ProcessMenu(ACTION_UP);
        btncounter--;
        break;
      case 4:
        clickedItem=menu.ProcessMenu(ACTION_BACK); 
        btncounter--;
        break;
    }
  }
  
  if (clickedItem > 0)
  {
    // Logic for root menu
    if (menu.CurrentMenu==mnuRoot)
      switch (clickedItem)
      {
        case 1:
          menu.InitMenu((const char ** )mnuWiFiTools,cntWiFiTools,1);
          break;
        case 2:
          menu.InitMenu((const char ** )mnuBlinky,cntBlinky,1);
          break;
        case 3:
          menu.InitMenu((const char ** )mnuGraphics,cntGraphics,1);
          break;
        case 4:
          menu.InitMenu((const char ** )mnuClient,cntClient,1);
          break; 
         case 5:
          menu.InitMenu((const char ** )mnuExtra,cntExtra,1);
          break;
         case 6:
          menu.InitMenu((const char ** )mnuSettings,cntSettings,1);
          break;
         case 7:
          menu.InitMenu((const char ** )mnuAbout,cntAbout,1);
          break;
      }

    // Logic for sub menus
    else if (menu.CurrentMenu==mnuWiFiTools)
      switch (clickedItem)
      {
        case 1:
          WiFi.forceSleepWake();
          WiFi.mode(WIFI_STA);
          menu.MessageBox("Scanning...");
          Channel_Activity();
          resetMenu();
          wifi_off();
          break;
        case 2:
          WiFi.forceSleepWake();
          WiFi.mode(WIFI_STA);
          menu.MessageBox("Scanning...");
          AP_Scanner();
          resetMenu();
          wifi_off();
          break;
      }

    else if (menu.CurrentMenu==mnuBlinky)
      switch (clickedItem)
      {
        case 1:
          mydisp.clearScreen();
          LED_WS2812FX_Favorites();
          resetMenu();
          break;
        case 2:
          mydisp.clearScreen();
          LED_Flashlight();
          resetMenu();
          break;
         case 3:
          mydisp.clearScreen();
          LED_WS2812FX_FullDemo();
          resetMenu();
          break;
         default:
          mydisp.clearScreen();
          ledmode = clickedItem - 3;
          LED_WS2812FX();
          resetMenu();
          break; 
      }

    else if (menu.CurrentMenu==mnuGraphics)
      switch (clickedItem)
      {
        case 1:
          Benchmark();
          break;
        case 2:
          break;
        case 3:
          break;
        case 4:
          break;
      }

    else if (menu.CurrentMenu==mnuClient)
      switch (clickedItem)
      {
        case 1:
          WiFi.forceSleepWake();
          WiFi.mode(WIFI_AP_STA);
          ConnectionManager();
          //if ap mode, display msg
          //if client mode, display msg
          menu.MessageBox("Connected");
          resetMenu();
          break;
        case 2:
          menu.MessageBox("Disconnected");
          WiFi.disconnect(); 
          break;
        case 3:
          break;
        case 4:
          menu.MessageBox("reset settings");
          wifiManager.resetSettings();
          break;
      }

    else if (menu.CurrentMenu==mnuExtra)
      switch (clickedItem)
      {
        case 1:
          menu.MessageBox("WiFi now off");
          wifi_off();
          break;
        case 2:
          menu.MessageBox("WiFi now off");
          wifi_off();
          break;
        case 3:
          break;
      }

    else if (menu.CurrentMenu==mnuSettings)
      switch (clickedItem)
      {
        case 1:
          //nick or alias setting placeholder
          appmode=1;
          //DebugInfo();
          appmode=0;
          break;
        case 2:
          //artwork setting placeholder
          break;
        case 3:
          menu.InitMenu((const char ** )mnuBrightness,cntBrightness,1);
          break;
        case 4:
          menu.InitMenu((const char ** )mnuRegion,cntRegion,1);
          break;
      }

    else if (menu.CurrentMenu==mnuBrightness)
      switch (clickedItem)
      {
        case 1:
          menu.MessageBox("Saved: Uber Low");
          pixels.setBrightness(10); 
          ws2812fx.setBrightness(10);
          mydisp.setBrightness(1);
          all_leds_off();
          EEPROM.write(PIXELBRIGHT_ADDR,10);
          EEPROM.write(MYDISPBRIGHT_ADDR,1);
          EEPROM.write(WS2812FXBRIGHT_ADDR,10);
          EEPROM.commit();
          break;
        case 2:
          menu.MessageBox("Saved: Low");
          pixels.setBrightness(64); //set to 1/4 brightness overall
          ws2812fx.setBrightness(64);
          mydisp.setBrightness(4);
          all_leds_off();
          EEPROM.write(PIXELBRIGHT_ADDR,64);
          EEPROM.write(MYDISPBRIGHT_ADDR,4);
          EEPROM.write(WS2812FXBRIGHT_ADDR,64);
          EEPROM.commit();
          break;
        case 3:
          menu.MessageBox("Saved: Medium");
          pixels.setBrightness(128);
          ws2812fx.setBrightness(128);
          mydisp.setBrightness(7);
          all_leds_off();
          EEPROM.write(PIXELBRIGHT_ADDR,128);
          EEPROM.write(MYDISPBRIGHT_ADDR,7);
          EEPROM.write(WS2812FXBRIGHT_ADDR,128);
          EEPROM.commit();
          break;
        case 4:
          menu.MessageBox("Saved: High");
          pixels.setBrightness(184);
          ws2812fx.setBrightness(184);
          mydisp.setBrightness(11);
          all_leds_off();
          EEPROM.write(PIXELBRIGHT_ADDR,184);
          EEPROM.write(MYDISPBRIGHT_ADDR,11);
          EEPROM.write(WS2812FXBRIGHT_ADDR,184);
          EEPROM.commit();
          break;
        case 5:
          menu.MessageBox("Be Careful");
          pixels.setBrightness(254);  // highest setting
          ws2812fx.setBrightness(254); // highest setting
          mydisp.setBrightness(15);
          all_leds_off();
          EEPROM.write(PIXELBRIGHT_ADDR,254);
          EEPROM.write(MYDISPBRIGHT_ADDR,15);
          EEPROM.write(WS2812FXBRIGHT_ADDR,254);
          EEPROM.commit();
          break;
      }
      
    else if (menu.CurrentMenu==mnuRegion)
      switch (clickedItem)
      {
        case 1:
        region_id = 1;  //US
        EEPROM.write(REGION_ADDR,region_id);  
        EEPROM.commit();
        menu.MessageBox("Region Set: US");
        break;
        
        case 2:
        region_id = 2;  //EU
        EEPROM.write(REGION_ADDR,region_id);  
        EEPROM.commit();
        menu.MessageBox("Region Set: EU");
        break;      
      }

    else if (menu.CurrentMenu==mnuAbout)
      switch (clickedItem)
      {
        case 1:
          SysInfo();
          break;
        case 2:
          IPConfig();
          break;
        case 3:
          Credits();
          break;
        case 4:
          Shouts();
          break;
        case 5:
          DebugInfo();
          break;
      } 
      
  }

  // menu return logic
  else if(clickedItem == -1){
    if (menu.CurrentMenu==mnuRoot)
      { //In root menu already - Do Nothing 
      }

    //2nd level menus
    else if (menu.CurrentMenu==mnuWiFiTools)   
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,1); }
    else if (menu.CurrentMenu==mnuBlinky)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,2); }
    else if (menu.CurrentMenu==mnuGraphics)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,3); }
    else if (menu.CurrentMenu==mnuClient)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,4); }
    else if (menu.CurrentMenu==mnuExtra)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,5); }
    else if (menu.CurrentMenu==mnuSettings)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,6); }
    else if (menu.CurrentMenu==mnuAbout)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,7); }

    //3rd level menus
    else if (menu.CurrentMenu==mnuBrightness)
      { menu.InitMenu((const char ** )mnuSettings,cntSettings,3); }
    else if (menu.CurrentMenu==mnuRegion)
      { menu.InitMenu((const char ** )mnuSettings,cntSettings,4); }

    // not converted yet
    else if (menu.CurrentMenu==(const char **)"SSID List")
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,3);
      }
   }
 }





