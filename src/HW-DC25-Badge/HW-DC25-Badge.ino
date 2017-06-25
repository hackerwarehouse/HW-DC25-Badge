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
#include "buttonecho.h"
#include "channelactivity.h"
#include "connectionmgr.h"
#include "core.h"
#include "mainmenu.h"
#include "settings.h"
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
volatile byte btnid  = 0;
volatile byte region_id = 1;
volatile byte mydispbrightness;

byte appmode = 0;
byte region_unlocked = 0;

long debouncing_time = 250;
unsigned long last_micros = 0;

void UP(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    if (appmode){      
    }
    else {
      btncounter ++;
    }
    btnid = 3;
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
    btnid = 2;
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
    btnid = 1;
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
    btnid = 4;
    last_micros = micros();
  }
}

void setup()
{  
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(left, INPUT);           //External pulldown resistor (required for programming)

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

void menu_reset(){
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

void menu_force_escape(){
  btnid = 4;
  btncounter++;
}

void loop()
{ 
  int keycode=0;
  int clickedItem=0; 
  
  if (btncounter > 0)
  { 
    switch(btnid)
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
          mydisp.setBrightness(mydispbrightness);
          break;
        case 2:
          menu.InitMenu((const char ** )mnuBlinky,cntBlinky,1);
          mydisp.setBrightness(mydispbrightness);
          break;
        case 3:
          menu.InitMenu((const char ** )mnuGraphics,cntGraphics,1);
          mydisp.setBrightness(mydispbrightness);
          break;
        case 4:
          menu.InitMenu((const char ** )mnuClient,cntClient,1);
          mydisp.setBrightness(mydispbrightness);
          break; 
         case 5:
          menu.InitMenu((const char ** )mnuExtra,cntExtra,1);
          mydisp.setBrightness(mydispbrightness);
          break;
         case 6:
          menu.InitMenu((const char ** )mnuSettings,cntSettings,1);
          mydisp.setBrightness(mydispbrightness);
          break;
         case 7:
          menu.InitMenu((const char ** )mnuAbout,cntAbout,1);
          mydisp.setBrightness(mydispbrightness);
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
          menu_reset();
          wifi_off();
          break;
        case 2:
          WiFi.forceSleepWake();
          WiFi.mode(WIFI_STA);
          menu.MessageBox("Scanning...");
          AP_Scanner();
          menu_reset();
          wifi_off();
          break;
      }

    else if (menu.CurrentMenu==mnuBlinky)
      switch (clickedItem)
      {
        case 1:
          mydisp.clearScreen();
          LED_WS2812FX_Favorites();
          menu_reset();
          break;
        case 2:
          mydisp.clearScreen();
          LED_Flashlight();
          menu_reset();
          break;
         case 3:
          mydisp.clearScreen();
          LED_WS2812FX_FullDemo();
          menu_reset();
          break;
         default:
          mydisp.clearScreen();
          byte ledmode = clickedItem - 3;
          LED_WS2812FX(ledmode);
          menu_reset();
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
          menu_reset();
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
          ButtonEcho();
          break;
        case 3:
          break;
      }

    else if (menu.CurrentMenu==mnuSettings)
      switch (clickedItem)
      {
        case 1:
          menu.InitMenu((const char ** )mnuBrightness,cntBrightness,1);
          mydisp.setBrightness(mydispbrightness);
          break;
        case 2:
          SelectArtwork();
          break;
        case 3:
          if (region_unlocked == 1) {
            menu.InitMenu((const char ** )mnuRegion,cntRegion,1);
            mydisp.setBrightness(mydispbrightness);
          }
          else { menu.MessageBox("Region is Locked"); }
          break;
        case 4:
          //nick or alias setting placeholder
          appmode=1;
          //DebugInfo();
          appmode=0;
          break;
      }

    else if (menu.CurrentMenu==mnuBrightness)
      switch (clickedItem)
      {
        case 1:
          SaveBrightness(10,1);
          menu.MessageBox("Saved: Uber Low");
          delay(1000);
          menu_force_escape();
          break;
        case 2:
          SaveBrightness(64,4);
          menu.MessageBox("Saved: Low");
          delay(1000);
          menu_force_escape();
          break;
        case 3:
          SaveBrightness(128,7);
          menu.MessageBox("Saved: Medium");
          delay(1000);
          menu_force_escape();
          break;
        case 4:
          SaveBrightness(184,11);
          menu.MessageBox("Saved: High");
          delay(1000);
          menu_force_escape();
          break;
        case 5:
          SaveBrightness(254,15);
          menu.MessageBox("Be Careful");
          delay(1000);
          menu_force_escape();
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
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,1); 
        mydisp.setBrightness(mydispbrightness);}
    else if (menu.CurrentMenu==mnuBlinky)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,2); 
        mydisp.setBrightness(mydispbrightness);}
    else if (menu.CurrentMenu==mnuGraphics)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,3); 
        mydisp.setBrightness(mydispbrightness);}
    else if (menu.CurrentMenu==mnuClient)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,4); 
        mydisp.setBrightness(mydispbrightness);}
    else if (menu.CurrentMenu==mnuExtra)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,5); 
        mydisp.setBrightness(mydispbrightness);}
    else if (menu.CurrentMenu==mnuSettings)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,6); 
        mydisp.setBrightness(mydispbrightness);}
    else if (menu.CurrentMenu==mnuAbout)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,7); 
        mydisp.setBrightness(mydispbrightness);}

    //3rd level menus
    else if (menu.CurrentMenu==mnuBrightness)
      { menu.InitMenu((const char ** )mnuSettings,cntSettings,1); 
        mydisp.setBrightness(mydispbrightness);}
    else if (menu.CurrentMenu==mnuRegion)
      { menu.InitMenu((const char ** )mnuSettings,cntSettings,3); 
        mydisp.setBrightness(mydispbrightness);}

    // not converted yet
    else if (menu.CurrentMenu==(const char **)"SSID List")
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,3);
        mydisp.setBrightness(mydispbrightness); }
   }
 }





