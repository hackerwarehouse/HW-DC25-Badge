#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <SSD_13XX.h>
extern "C" {
  #include "user_interface.h"
}

#include "core.h"

extern SSD_13XX mydisp;
extern byte btncounter;
extern volatile byte btnid;
extern byte appmode;
extern byte region_id;


unsigned long pkts = 0;
unsigned long deauths = 0;
unsigned long maxVal = 0;
double multiplicator = 0.0;

int val[96];
int highval[14];

void sniffer(uint8_t *buf, uint16_t len) {
  pkts++;
  if(buf[12] == 0xA0 || buf[12] == 0xC0){
    deauths++;
  }
}

void getMultiplicator(){
  maxVal = 1;
  for(int i=1;i<97;i++){
    if(val[i] > maxVal) maxVal = val[i];
  }
  if(maxVal > 56) multiplicator = (double)56/(double)maxVal;
  else multiplicator = 1;
}

void chn_activity_display(byte curchn){
  mydisp.clearScreen();
  mydisp.setTextColor(WHITE);
  mydisp.setCursor(0, 0);
  mydisp.setTextScale(1);
  mydisp.println("Ch: " + (String)curchn + "           Pkts: " + (String)pkts);
  for(int i=1;i<96;i++) mydisp.drawLine(i, 56-val[i]*multiplicator, i, 56, BLUE);
}

void chn_activity_reset(byte curchn){
  wifi_set_channel(curchn);
  for(int i=1;i<97;i++) val[i] = 0;
  pkts = 0;
  deauths = 0;
  multiplicator = 1;
  chn_activity_display(curchn);
}

void Pkt_Monitor(){
  appmode=1;
  btnid = 0;
  
  byte curchn = 1;
  byte maxCh;
  unsigned long prevTime = 0;
  unsigned long curTime = 0;

  if (region_id == 1) { maxCh = 11; }
  else if (region_id == 2) { maxCh = 13; }

  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(0);
  WiFi.disconnect();
  wifi_set_promiscuous_rx_cb(sniffer);
  wifi_set_channel(curchn);
  wifi_promiscuous_enable(1);
  chn_activity_reset(1);
  
  while (1)
  {
    if (btnid == 3){ 
      if (curchn == maxCh) {curchn = 1; } 
      else {curchn++;}
      chn_activity_reset(curchn);
      btnid = 0;
    }
    else if (btnid == 2){ 
      if (curchn == 1) {curchn = maxCh; } 
      else {curchn--;}
      chn_activity_reset(curchn);
      btnid = 0;
    }
    else if (btnid == 4) {break;}

    curTime = millis();
    if(curTime - prevTime >= 1000){
      prevTime = curTime;
  
      //move every packet bar one pixel to the left
      for(int i=1;i<97;i++){
        val[i] = val[i+1];
      }
      val[96] = pkts;
  
      getMultiplicator();
      chn_activity_display(curchn);

      //reset counters
      pkts = 0;
      deauths = 0;
    }
    delay(100);
  }
  wifi_promiscuous_enable(0);
  appmode=0;
  btncounter++;
}


void chn_deauth_monitor_display(byte curchn){
  mydisp.clearScreen();
  mydisp.setTextColor(RED);
  mydisp.setCursor(0, 0);
  mydisp.setTextScale(1);
  mydisp.println("Ch: " + (String)curchn + "    Deauths: " + (String)deauths);
  for(int i=1;i<96;i++) mydisp.drawLine(i, 56-val[i]*multiplicator, i, 56, RED);
}

void chn_deauth_monitor_reset(byte curchn){
  wifi_set_channel(curchn);
  for(int i=1;i<97;i++) val[i] = 0;
  pkts = 0;
  deauths = 0;
  multiplicator = 1;
  chn_deauth_monitor_display(curchn);
}

void DeauthMonitor(){
  appmode=1;
  btnid = 0;
  
  byte curchn = 1;
  byte maxCh;
  unsigned long prevTime = 0;
  unsigned long curTime = 0;

  if (region_id == 1) { maxCh = 11; }
  else if (region_id == 2) { maxCh = 13; }

  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(0);
  WiFi.disconnect();
  wifi_set_promiscuous_rx_cb(sniffer);
  wifi_set_channel(curchn);
  wifi_promiscuous_enable(1);
  chn_deauth_monitor_reset(1);
  
  while (1)
  {
    if (btnid == 3){ 
      if (curchn == maxCh) {curchn = 1; } 
      else {curchn++;}
      chn_deauth_monitor_reset(curchn);
      btnid = 0;
    }
    else if (btnid == 2){ 
      if (curchn == 1) {curchn = maxCh; }
      else {curchn--;}
      chn_deauth_monitor_reset(curchn);
      btnid = 0;
    }
    else if (btnid == 4) {break;}

    curTime = millis();
    if(curTime - prevTime >= 1000){
      prevTime = curTime;

      //move every packet bar one pixel to the left
      for(int i=1;i<97;i++){
        val[i] = val[i+1];
      }
      val[96] = deauths;

      getMultiplicator();
      chn_deauth_monitor_display(curchn);

      //reset counters
      deauths = 0;
      pkts = 0;
    }
    delay(100);
  }
  wifi_promiscuous_enable(0);
  appmode=0;
  btncounter++;
}

void chn_deauth_detect_display(){
  mydisp.clearScreen();
  mydisp.setTextColor(RED);
  mydisp.setCursor(0, 0);
  mydisp.setTextScale(1);
  mydisp.println(F("Chn: current / high"));
  mydisp.println(" 1: " + (String)val[1] + "/" + (String)highval[1] + "     8: " + (String)val[8] + "/" + (String)highval[8]);
  mydisp.println(" 2: " + (String)val[2] + "/" + (String)highval[2] + "     9: " + (String)val[9] + "/" + (String)highval[9]);
  mydisp.println(" 3: " + (String)val[3] + "/" + (String)highval[3] + "    10: " + (String)val[10] + "/" + (String)highval[10]);
  mydisp.println(" 4: " + (String)val[4] + "/" + (String)highval[4] + "    11: " + (String)val[11] + "/" + (String)highval[11]);
  if (region_id == 1) { mydisp.println(" 5: " + (String)val[5] + "/" + (String)highval[5]); }
  else { mydisp.println(" 5: " + (String)val[5] + "/" + (String)highval[5] + "    12: " + (String)val[12] + "/" + (String)highval[12]); }
  if (region_id == 1) { mydisp.println(" 6: " + (String)val[6] + "/" + (String)highval[6]); }
  else { mydisp.println(" 6: " + (String)val[6] + "/" + (String)highval[6] + "    13: " + (String)val[13] + "/" + (String)highval[13]); }
  if (region_id == 3) { mydisp.println(" 7: " + (String)val[7] + "/" + (String)highval[7] + "    14: " + (String)val[14] + "/" + (String)highval[14]); }
  else { mydisp.println(" 7: " + (String)val[7] + "/" + (String)highval[7]); }
}

void DeauthDetect(){
  appmode=1;
  btnid = 0;
  
  byte curchn = 1;
  byte maxCh;
  unsigned long prevTime = 0;
  unsigned long curTime = 0;

  if (region_id == 1) { maxCh = 11; }
  else if (region_id == 2) { maxCh = 13; }

  for(int i=1;i<maxCh;i++){
    val[i] = 0;
    highval[i] = 0;
  }

  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(0);
  WiFi.disconnect();
  wifi_set_promiscuous_rx_cb(sniffer);
  wifi_set_channel(curchn);
  wifi_promiscuous_enable(1);
  chn_activity_reset(1);
  
  while (1)
  {
    if (btnid == 4) {break;}

    curTime = millis();
    if(curTime - prevTime >= 1000){
      prevTime = curTime;

      val[curchn] = deauths;
      if (highval[curchn] > val[curchn]) { highval[curchn] = deauths; }
      chn_deauth_detect_display();

      if (curchn < maxCh) { curchn++; }
      else if (curchn == maxCh) { curchn = 1;}
      wifi_set_channel(curchn);

      //reset counters
      deauths = 0;
      pkts = 0;
    }
    delay(100);
  }
  wifi_promiscuous_enable(0);
  appmode=0;
  btncounter++;
}

