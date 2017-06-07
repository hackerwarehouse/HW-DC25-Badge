#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <SSD_13XX.h>
#include <WiFiClient.h>

#include "_fonts/channels.c"
#include "core.h"

extern SSD_13XX mydisp;
extern Adafruit_NeoPixel pixels;
extern byte counter;

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

int chn [13];
long times [13];
int mult = 5;

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
