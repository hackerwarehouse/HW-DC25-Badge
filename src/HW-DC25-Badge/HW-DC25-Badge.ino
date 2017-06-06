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
#include "_fonts/channels.c"
#include "_fonts/defaultFont.c"

#include "blinky.h"
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

long debouncing_time = 250;
long interavl = 250;
long interval1 = 500;
long times [13];

unsigned long last_micros = 0;
unsigned long prevMillis = 0;

int delayval = 500;
int chn [13];
int mult = 5;

bool flag = true;

class Flasher
{
  // Class Member Variables
  // These are initialized at startup
  int ledPin;      // the number of the LED pin
  long OnTime;     // milliseconds of on-time
  long OffTime;    // milliseconds of off-time
  
  // These maintain the current state
  bool ledState;                 // ledState used to set the LED
  unsigned long previousMillis;   // will store last time LED was updated
 
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int pin, long ON, long OFF)
  {
  ledPin = pin;     
    
  OnTime = ON;
  OffTime = OFF;
  
  ledState = false; 
  previousMillis = 0;
  }
 
  void Update()
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if((ledState == true) && (currentMillis - previousMillis >= OnTime) || OnTime==0)
    {
      ledState = false;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      pixels.setPixelColor(ledPin, pixels.Color(0,0,0));
      pixels.show();
    }
    else if ((ledState == false) && (currentMillis - previousMillis >= OffTime) && OffTime!=0)
    {
      ledState = true;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      if(OnTime==1000){
        pixels.setPixelColor(ledPin, pixels.Color(0,255,0));
        pixels.show();
      }
      else if(OnTime==500){
        pixels.setPixelColor(ledPin, pixels.Color(255,255,0));
        pixels.show();
      }
      else if (OnTime == 100){
        pixels.setPixelColor(ledPin, pixels.Color(255,0,0));
        pixels.show();
      }
    }  
  }
};

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

//AP Scanner
void Scanner(){
  int n = WiFi.scanNetworks();
  mydisp.clearScreen();
  for (int i=0; i<n; i++){
    int x = random(0,65535);
    String Name = WiFi.SSID(i);
    mydisp.setTextColor(x);
    mydisp.println(Name);
  }
}

//Channel Traffic 
void Channel_Activity(){
 for (int i=0; i<NUMPIXELS;i++){
  chn[i] = 0;
  times[i] = 0;
  }

  //Count number of networks in each channel
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    String Name = WiFi.SSID(i);
    int count = WiFi.channel(i);
    Serial.println(Name + ": " + count);
    switch (count){
    case 1:
    {
      chn[0] += 1;
    }
    break;
    case 2:
    {
      chn[1] += 1;  
    }
    break;
    case 3:
    {
      chn[2] += 1;
    }
    break;
    case 4:
    {
      chn[3] += 1;
    }
    break;
    case 5:
    {
      chn[4] += 1;
    }
    break;
    case 6:
    {
      chn[5] += 1;
    }
    break;
    case 7:
    {
      chn[6] += 1;
    }
    break;
    case 8:
    {
      chn[7] += 1;
    }
    break;
    case 9:
    {
      chn[8] += 1;
    }
    break;
    case 10:
    {
      chn[9] += 1;
    }
    break;
    case 11:
    {
      chn[10] += 1;     
    }
    break;
    case 12:
    {
      chn[11] += 1;
    }
    break;
    case 13:
    {
      chn[12] += 1;
    }
    break;
    case 14:
    {
      chn[13] += 1;
    }
    break;
    }
  }

  //Assign delay time for blinking depending on number of networks in each channel
  for (int i =0; i<NUMPIXELS; i++){
    if(chn[i]==0){
      times[i] = 0;
    }
    else if(chn[i]==1 || chn[i]==2){
      times[i] = 1000;
    }
    else if(chn[i]==3 || chn[i]==4){
      times[i] = 500;
    }
    else if(chn[i]>=5){
      times[i] = 100;
    } 
  }

  //Create intance of Flasher class for each LED. 
  Flasher led1(0, times[0], times[0]);
  Flasher led2(1, times[1], times[1]);
  Flasher led3(2, times[2], times[2]);
  Flasher led4(3, times[3], times[3]);
  Flasher led5(4, times[4], times[4]);
  Flasher led6(5, times[5], times[5]);
  Flasher led7(6, times[6], times[6]);
  Flasher led8(7, times[7], times[7]);
  Flasher led9(8, times[8], times[8]);
  Flasher led10(9, times[9], times[9]);
  Flasher led11(10, times[10], times[10]);
  Flasher led12(11, times[11], times[11]);
  Flasher led13(12, times[12], times[12]);
  Flasher led14(13, times[13], times[13]);

  //Set Title, font and horizontal line 
  mydisp.clearScreen();
  mydisp.setCursor(14, 0);
  mydisp.setTextColor(CYAN);
  mydisp.print("Channel Activity");
  mydisp.setFont(&channels);
  mydisp.setTextColor(WHITE);
  mydisp.setCursor(0,53);
  mydisp.print("1 2 3 4 5 6 7 8 9 10 11 12 13");
  mydisp.drawLine(0, 53, 95, 53, ORANGE);

  //Draw Bar Graph
  for(int i =0; i<14; i++){
    if(i<9){
      if(times[i]==1000){
        mydisp.fillRect(i*6,53-chn[i]*mult,3,chn[i]*mult,GREEN);
        mydisp.setCursor(i*6-1, (53-chn[i]*mult)-10);
        mydisp.print(chn[i]);
      }
      else if(times[i]==500){
        mydisp.fillRect(i*6,53-chn[i]*mult,3,chn[i]*mult,YELLOW);
        mydisp.setCursor(i*6-1, (53-chn[i]*mult)-10);
        mydisp.print(chn[i]);
      }
      else if(times[i]==100){
        mydisp.fillRect(i*6,53-chn[i]*mult,3,chn[i]*mult,RED);
        mydisp.setCursor(i*6-1, (53-chn[i]*mult)-10);
        mydisp.print(chn[i]);
      }
    }

    //Change spacing between bars after channel 9
    else if(i>=9){
      if(times[i]==1000){
        mydisp.fillRect(10*(i-4)+7,53-chn[i]*mult,3,chn[i]*mult,GREEN);
        mydisp.setCursor((10*(i-4)+7), (53-chn[i]*mult)-10);
        mydisp.print(chn[i]);
      }
      else if(times[i]==500){
        mydisp.fillRect(10*(i-4)+7,53-chn[i]*mult,3,chn[i]*mult,YELLOW);
        mydisp.setCursor((10*(i-4)+7), (53-chn[i]*mult)-10);
        mydisp.print(chn[i]);
      }
      else if(times[i]==100){
        mydisp.fillRect(10*(i-4)+7,53-chn[i]*mult,3,chn[i]*mult,RED);
        mydisp.setCursor((10*(i-4)+7), (53-chn[i]*mult)-10);
        mydisp.print(chn[i]);
      }
    }
  }

  //Blink LEDs 
  while (counter == 0){
    yield();
    led1.Update();
    led2.Update();
    led3.Update();
    led4.Update();
    led5.Update();
    led6.Update();
    led7.Update();
    led8.Update();
    led9.Update();
    led10.Update();
    led11.Update();
    led12.Update();
    led13.Update();
    led14.Update();
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





