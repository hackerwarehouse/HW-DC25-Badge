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

#include "TestMenu.h"
#include "_images/badge.c"
#include "_images/hacker.c"
#include "_images/godai.c"
#include "_images/js.c"
#include "_images/garret.c"
#include "_fonts/defaultFont.c"

#include "apscanner.h"
#include "blinky.h"
#include "channelactivity.h"
#include "core.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
SSD_13XX mydisp(_CS, _DC);
qMenuSystem menu=qMenuSystem(&mydisp);

const byte left = 15; 
const byte down = 10; 
const byte right = 2; 
const byte up = 0;  

volatile byte counter = 0;
volatile byte id  = 0;

//cant find where used
//long interavl = 250;
//long interval1 = 500;
//unsigned long prevMillis = 0;
//int delayval = 500;
//bool flag = true;

long debouncing_time = 250;
unsigned long last_micros = 0;

enum Tasks {Null,
            Random,
            Cyclon,
            Chase,
            Flashlight,
            Scan,
            Channel
            };

Tasks CurrentTask = Null; 

void turn_off(){
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
  }  
}

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

 /* WiFi.mode (WIFI_STA);
  WiFi.disconnect();*/

  /*WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();*/
  
  menu.InitMenu((const char **)mnuRoot,cntRoot,1);
  pixels.begin();
  turn_off();
  Serial.begin(115200);
}

void loop()
{ 
  int keycode=0;
  int clickedItem=0; 

  if (CurrentTask == Random){
    LED1();
    if (counter > 0){
      turn_off();   
      CurrentTask = Null;
    }  
  }

  else if (CurrentTask == Cyclon){
    LED2();
    if (counter > 0){
      turn_off();   
      CurrentTask = Null;
    }
  }

  else if (CurrentTask == Chase){
    LED3();
    if (counter > 0){
      turn_off();   
      CurrentTask = Null;
    }
  }

  else if (CurrentTask == Flashlight){
    LED4();
    if (counter > 0){
      turn_off();  
      CurrentTask = Null;
    }
  }
  
  else if (CurrentTask == Scan){
    Scanner();
    CurrentTask = Null;
    ///WiFi.mode(WIFI_OFF);
    //WiFi.forceSleepBegin();   
  }

  else if (CurrentTask == Channel){
    Channel_Activity();
    turn_off();
    mydisp.setFont(&defaultFont);
    CurrentTask = Null;   
  }
  
  else if (counter > 0 && CurrentTask == Null)
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
          menu.InitMenu((const char ** )mnuSubmenu1,cntSubmenu1,1);
          break;
        case 2:
          menu.InitMenu((const char ** )mnuSubmenu2,cntSubmenu2,1);
          break;
        case 3:
          menu.InitMenu((const char ** )mnuSubmenu3,cntSubmenu3,1);
          break;
        case 4:
          menu.InitMenu((const char ** )mnuSubmenu4,cntSubmenu4,1);
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
      
    // Logic for Submenu 1
    else if (menu.CurrentMenu==mnuSubmenu1)
      switch (clickedItem)
      {
        case 1:
        menu.MessageBox("Scanning...");
        CurrentTask = Channel;
        break;
        case 2:
        mydisp.clearScreen();
        mydisp.setRotation(0);
        mydisp.drawImage(11, 0, &badge);
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
          CurrentTask = Scan;
          break;
      }
      
    // Logic for Submenu 4
    else if (menu.CurrentMenu==mnuSubmenu4)
      switch (clickedItem)
      {
        case 1:
          menu.MessageBox("");
          CurrentTask=Random;
          break;
        case 2:
          menu.MessageBox("");
          CurrentTask=Cyclon;  
          break;
         case 3:
          menu.MessageBox("");
          CurrentTask=Chase;  
          break;
         case 4:
          menu.MessageBox("");
          CurrentTask=Flashlight;  
          break;
      }

      // Logic for Submenu 5
    else if (menu.CurrentMenu==mnuSubmenu5)
      switch (clickedItem)
      {
        case 1:
        mydisp.clearScreen();
        mydisp.setRotation(0);
        mydisp.setTextColor(GREENYELLOW);
        mydisp.drawImage(0, 0, &js);
        mydisp.setCursor(17, 4);
        mydisp.print("Jaycon Systems");
        mydisp.drawImage(1, 16, &hacker);
        mydisp.setCursor(17, 20);
        mydisp.print("Hacker Warehouse");
        mydisp.drawImage(1, 32, &godai);
        mydisp.setCursor(17, 36);
        mydisp.print("Godai Group");
        mydisp.drawImage(1, 48, &garret);
        mydisp.setCursor(17, 52);
        mydisp.print("Garrett Gee 2");
        
          break;
        case 2:
        mydisp.clearScreen();
        mydisp.setRotation(0);
        mydisp.setTextColor(CYAN);
        mydisp.setCursor(0, 4);
        mydisp.print("jayconsystems.com");
        mydisp.drawLine(0, 12, 86, 12, CYAN);
        mydisp.setCursor(0, 20);
        mydisp.print("hackerwarehouse.com");
        mydisp.drawLine(0, 28, 93, 28, CYAN);
        mydisp.setCursor(0, 36);
        mydisp.print("godaigroup.net");
        mydisp.drawLine(0, 44, 60, 44, CYAN);
        mydisp.setCursor(0, 52);
        mydisp.print("garrettgee.com");
        mydisp.drawLine(0, 60, 64, 60, CYAN);

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
        //Do Nothing
      }

    // Logic for Submenu 1
    else if (menu.CurrentMenu==mnuSubmenu1)   
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,1);
      }

    // Logic for Submenu 2
    else if (menu.CurrentMenu==mnuSubmenu2)
      {     
        menu.InitMenu((const char ** )mnuRoot,cntRoot,2);
      }

    // Logic for Submenu 3
    else if (menu.CurrentMenu==mnuSubmenu3)
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,3);
      }

    // Logic for Submenu 4
    else if (menu.CurrentMenu==mnuSubmenu4)
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,4);
      }

    // Logic for Submenu 5
    else if (menu.CurrentMenu==mnuSubmenu5)
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,5);
      }

     // Logic for Submenu 6
    else if (menu.CurrentMenu==mnuSubmenu6)
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,6);
      }

    // Logic for Submenu 7
    else if (menu.CurrentMenu==mnuSubmenu7)
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,7);
      }

    else if (menu.CurrentMenu==(const char **)"SSID List")
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,3);
      }
   }
 }





