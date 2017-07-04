#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <SSD_13XX.h>
#include <WiFiClient.h>

#include "_fonts/channels.c"
#include "core.h"

extern SSD_13XX mydisp;
extern Adafruit_NeoPixel pixels;
extern byte btncounter;
extern byte region_id;

byte chn [14];
byte mult = 5;

void AP_Channel_Usage(){
 
  for (int i=0; i<NUMPIXELS;i++){ chn[i] = 0; }

  //Count number of networks in each channel
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    String Name = WiFi.SSID(i);
    int count = WiFi.channel(i);
    Serial.println(Name + ": " + count);
    switch (count){
      default:
        chn[count] += 1;
        break;
      }
  }

  //Set Title, font and horizontal line 
  mydisp.clearScreen();
  mydisp.setCursor(14, 0);
  mydisp.setTextColor(CYAN);
  mydisp.print(F("AP Channel Usage"));
  mydisp.setFont(&channels);
  mydisp.setTextColor(WHITE);
  mydisp.setCursor(0,53);
  if (region_id == 1){  //US
    mydisp.print(F("1 2 3 4 5 6 7 8 9 AB"));
    mydisp.print(F(" "));
    mydisp.setTextColor(RED);
    mydisp.print(F("12 13"));
  }
  else if (region_id == 2)  //EU
    mydisp.print(F("1 2 3 4 5 6 7 8 9 AB CD"));
  mydisp.drawLine(0, 53, 95, 53, ORANGE);
  mydisp.setTextColor(WHITE);

  //Draw Bar Graph
  byte num_chns;
  if(region_id == 1) //US
     num_chns = 11;
  else if (region_id == 2) // EU
     num_chns = 13;
  for(int i=1; i<num_chns; i++){
    if(chn[i] == 0){ }
    else if(chn[i] <= 2){
      mydisp.fillRect((i-1)*6,53-chn[i]*mult,3,chn[i]*mult,GREEN);
      mydisp.setCursor((i-1)*6-1, (53-chn[i]*mult)-10);
      mydisp.print(chn[i]);
    }
    else if(chn[i] <= 4){
      mydisp.fillRect((i-1)*6,53-chn[i]*mult,3,chn[i]*mult,YELLOW);
      mydisp.setCursor((i-1)*6-1, (53-chn[i]*mult)-10);
      mydisp.print(chn[i]);
    }
    else {
      mydisp.fillRect((i-1)*6,53-chn[i]*mult,3,chn[i]*mult,RED);
      mydisp.setCursor((i-1)*6-1, (53-chn[i]*mult)-10);
      mydisp.print(chn[i]);
    }
  }
}
