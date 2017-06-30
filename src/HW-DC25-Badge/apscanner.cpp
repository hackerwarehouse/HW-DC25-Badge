#include <ESP8266WiFi.h>
#include <SSD_13XX.h>
#include <WiFiClient.h>

#include "core.h"

extern SSD_13XX mydisp;
extern byte btncounter;
extern volatile byte btnid;
extern byte appmode;

void AP_Scanner(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  
  appmode=1;
  byte count=0;
  byte last=1;
  btnid = 0;

  int n = WiFi.scanNetworks();
  mydisp.clearScreen();
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  
  if (n == 0){
    mydisp.clearScreen();
    mydisp.println(F("No SSIDs detected :("));
    delay(1000);
    btnid=4;
  }
  else {
//    mydisp.clearScreen();
//    for (int i=0; i<n; i++){
//      mydisp.print(WiFi.RSSI(i));
//      mydisp.print(F(" | "));
//      mydisp.println(WiFi.SSID(i));
//    }
  }
  
  while (1)
  {
    if (btnid == 3){ 
      if (count == 0) {count = n-1; } 
      else {count--;}
      btnid = 0;
    }
    else if (btnid == 2){ 
      if (count == n-1) {count = 0; } 
      else {count++;}
      btnid = 0;
    }
    else if (btnid == 4){
      //left button -> promopt refresh or exit?
      break;
    }

    if (count != last)
    {
      mydisp.clearScreen();
      mydisp.print(F("....:::: SSID "));
      mydisp.print(count + 1);
      mydisp.print(F(" of "));
      mydisp.print(n);
      mydisp.println(F(" ::::...."));
      mydisp.println();

      mydisp.print(F("ssid: "));
      mydisp.println(WiFi.SSID(count));
      
      mydisp.print(F("channel: "));
      mydisp.println(WiFi.channel(count));
      mydisp.print(F("dBm: "));
      mydisp.println(WiFi.RSSI(count));
      mydisp.print(F("enc: "));
      mydisp.println((WiFi.encryptionType(count) == ENC_TYPE_NONE)?"no":"yes");
      last = count;
    }
    delay(100);
  }
  appmode=0;
  btncounter++;
}

