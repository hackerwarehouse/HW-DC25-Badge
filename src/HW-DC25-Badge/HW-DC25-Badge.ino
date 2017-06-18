#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SSD_13XX.h>
#include <SPI.h>
#include <qMenuDisplay.h>
#include <qMenuSystem.h>
#include "_fonts/defaultFont.c"

#include "about.h"
#include "apscanner.h"
#include "benchmark.h"
#include "blinky.h"
#include "channelactivity.h"
#include "core.h"
#include "mainmenu.h"
#include "shouts.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
SSD_13XX mydisp(_CS, _DC);
qMenuSystem menu=qMenuSystem(&mydisp);

const byte left = 15; 
const byte down = 10; 
const byte right = 2; 
const byte up = 0;  

volatile byte counter = 0;
volatile byte id  = 0;

long debouncing_time = 250;
unsigned long last_micros = 0;

void UP(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
  counter ++;
  id = 3;
  last_micros = micros();
  }
}

void DOWN(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
  counter ++;
  id = 2;
  last_micros = micros();
  }
}

void RIGHT(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
  counter ++;
  id = 1;
  last_micros = micros();
  }
}

void LEFT(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
  counter ++;
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
  pixels.setBrightness(64); //set to 1/4 brightness overall
  mydisp.setBrightness(4);
  all_leds_off();
  Serial.begin(115200);
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
}

void loop()
{ 
  int keycode=0;
  int clickedItem=0; 
  
  if (counter > 0)
  { 
    switch(id)
    {
      case 1:       
        clickedItem=menu.ProcessMenu(ACTION_SELECT);
        counter--;
        break;
      case 2:
        menu.ProcessMenu(ACTION_DOWN);
        counter--;
        break;      
      case 3:
        menu.ProcessMenu(ACTION_UP);
        counter--;
        break;
      case 4:
        clickedItem=menu.ProcessMenu(ACTION_BACK); 
        counter--;
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
          menu.InitMenu((const char ** )mnuExtra,cntExtra,1);
          break; 
         case 5:
          menu.InitMenu((const char ** )mnuSettings,cntSettings,1);
          break;
         case 6:
          menu.InitMenu((const char ** )mnuAbout,cntAbout,1);
          break;
      }

    // Logic for sub menus
    else if (menu.CurrentMenu==mnuWiFiTools)
      switch (clickedItem)
      {
        case 1:
          menu.MessageBox("Scanning...");
          Channel_Activity();
          resetMenu();
          break;
        case 2:
          menu.MessageBox("Scanning...");
          AP_Scanner();
          resetMenu();
          //wifi_off();
          break;
      }

    else if (menu.CurrentMenu==mnuBlinky)
      switch (clickedItem)
      {
        case 1:
          mydisp.clearScreen();
          LED_Random();
          resetMenu();
          break;
        case 2:
          menu.MessageBox(""); 
          LED_Larson();
          resetMenu();
          break;
         case 3:
          menu.MessageBox("");
          LED_Chase();
          resetMenu();
          break;
         case 4:
          menu.MessageBox("");
          LED_Flashlight();
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

    else if (menu.CurrentMenu==mnuExtra)
      switch (clickedItem)
      {
        case 1:
          Shouts();
          break;
        case 2:
          mydisp.clearScreen();
          break;
      }

    else if (menu.CurrentMenu==mnuSettings)
      switch (clickedItem)
      {
        case 1:
          //nick or alias setting placeholder
          break;
        case 2:
          //artwork setting placeholder
          break;
        case 3:
          //Setting_Brightness();
          menu.InitMenu((const char ** )mnuBrightness,cntBrightness,1);
          break;
      }

    else if (menu.CurrentMenu==mnuBrightness)
      switch (clickedItem)
      {
        case 1:
          menu.MessageBox("Saved: Uber Low");
          pixels.setBrightness(10); 
          mydisp.setBrightness(1);
          break;
        case 2:
          menu.MessageBox("Saved: Low");
          pixels.setBrightness(64); //set to 1/4 brightness overall
          mydisp.setBrightness(4);
          break;
        case 3:
          menu.MessageBox("Saved: Medium");
          pixels.setBrightness(128);
          mydisp.setBrightness(7);
          break;
        case 4:
          menu.MessageBox("Saved: High");
          pixels.setBrightness(184);
          mydisp.setBrightness(11);
          break;
        case 5:
          menu.MessageBox("Be Careful!");
          pixels.setBrightness(255);  // highest setting
          mydisp.setBrightness(15);
          break;
      }

    else if (menu.CurrentMenu==mnuAbout)
      switch (clickedItem)
      {
        case 1:
          SysInfo();
          break;
        case 2:
          Credits();
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
    else if (menu.CurrentMenu==mnuExtra)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,4); }
    else if (menu.CurrentMenu==mnuSettings)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,5); }
    else if (menu.CurrentMenu==mnuAbout)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,6); }

    //3rd level menu
    else if (menu.CurrentMenu==mnuBrightness)
      { menu.InitMenu((const char ** )mnuSettings,cntSettings,1); }

    // not converted yet
    else if (menu.CurrentMenu==(const char **)"SSID List")
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,3);
      }
   }
 }





