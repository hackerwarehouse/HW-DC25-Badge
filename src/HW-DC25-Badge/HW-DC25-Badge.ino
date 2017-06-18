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

#include "_images/badge.c"
#include "_fonts/defaultFont.c"

#include "apscanner.h"
#include "blinky.h"
#include "credits.h"
#include "channelactivity.h"
#include "core.h"
#include "mainmenu.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
SSD_13XX mydisp(_CS, _DC);
qMenuSystem menu=qMenuSystem(&mydisp);

//cant find where used
//long interavl = 250;
//long interval1 = 500;
//unsigned long prevMillis = 0;
//int delayval = 500;
//bool flag = true;

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

void resetMenu(){
  all_leds_off();
  mydisp.setFont(&defaultFont);
//  CurrentTask = Null;
//  wifi_off();
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
  all_leds_off();
  Serial.begin(115200);
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
    // Logic for Root menu
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
          menu.InitMenu((const char ** )mnuSubmenu3,cntSubmenu3,1);
          break;
        case 4:
          menu.InitMenu((const char ** )mnuSubmenu2,cntSubmenu2,1);
          break; 
         case 5:
          menu.InitMenu((const char ** )mnuSubmenu5,cntSubmenu5,1);
          break;
         case 6:
          menu.InitMenu((const char ** )mnuSubmenu6,cntSubmenu6,1);
          break;
         case 7:
          menu.InitMenu((const char ** )mnuSubmenu7,cntSubmenu7,1);
          break;
      }
      
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
          menu.MessageBox("");
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

      
      
    // Logic for Submenu 2
    else if (menu.CurrentMenu==mnuSubmenu2)
      switch (clickedItem)
      {
        case 1:
        case 2:
          break;
      }
      
    // Logic for Submenu 3
    else if (menu.CurrentMenu==mnuSubmenu3)
      switch (clickedItem)
      {
        case 1:
          menu.MessageBox("Scanning...");
          break;
      }

      // Logic for Submenu 5
    else if (menu.CurrentMenu==mnuSubmenu5)
      switch (clickedItem)
      {
        case 1:
          Credits();
          break;
        case 2:
          mydisp.clearScreen();
      }

      // Logic for Submenu 6
    else if (menu.CurrentMenu==mnuSubmenu6)
      switch (clickedItem)
      {
        case 1:
        case 2:
        case 3:
          break;
      }

      // Logic for Submenu 7
    else if (menu.CurrentMenu==mnuSubmenu7)
      switch (clickedItem)
      {
        case 1:
        case 2:
        case 3:
          break;
      }
  } 
  
  else if(clickedItem == -1){ // Return Logic
    // Logic for Submenu 1
    if (menu.CurrentMenu==mnuRoot)
      {
        menu.MessageBox("I am in root menu");
        //Do Nothing
      }

    else if (menu.CurrentMenu==mnuWiFiTools)   
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,1); }
    else if (menu.CurrentMenu==mnuBlinky)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,2); }

    // not converted yet
    else if (menu.CurrentMenu==mnuSubmenu3)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,3); }
    else if (menu.CurrentMenu==mnuSubmenu2)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,4); }
    else if (menu.CurrentMenu==mnuSubmenu5)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,5); }
    else if (menu.CurrentMenu==mnuSubmenu6)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,6); }
    else if (menu.CurrentMenu==mnuSubmenu7)
      { menu.InitMenu((const char ** )mnuRoot,cntRoot,7); }

    else if (menu.CurrentMenu==(const char **)"SSID List")
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,3);
      }
   }
 }





