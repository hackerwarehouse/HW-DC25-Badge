#include <EEPROM.h>

#include <ESP8266WiFi.h>
#include <SSD_13XX.h>
extern "C" {
  #include <user_interface.h>
}

#include "core.h"
#include "structures.h"

extern SSD_13XX mydisp;
extern volatile byte btnid;
extern byte appmode;
extern byte btncounter;
extern byte region_id;

#define DATA_LENGTH           112
#define TYPE_MANAGEMENT       0x00
#define TYPE_CONTROL          0x01
#define TYPE_DATA             0x02
#define SUBTYPE_PROBE_REQUEST 0x04
#define SUBTYPE_BEACON        0x08

struct SnifferPacket{
    struct RxControl rx_ctrl;
    uint8_t data[DATA_LENGTH];
    uint16_t cnt;
    uint16_t len;
};

static void getMAC(char *addr, uint8_t* data, uint16_t offset) {
  sprintf(addr, "%02x:%02x:%02x:%02x:%02x:%02x", data[offset+0], data[offset+1], data[offset+2], data[offset+3], data[offset+4], data[offset+5]);
}

static String getSSID(uint16_t start, uint16_t size, uint8_t* data) {
    String ssid = "";
    char letter;
    for(uint16_t i = start; i < DATA_LENGTH && i < start+size; i++) {
        letter = data[i];
        ssid += letter;
    }
    return ssid;
}

static void printDataSpan(uint16_t start, uint16_t size, uint8_t* data) {
  for(uint16_t i = start; i < DATA_LENGTH && i < start+size; i++) {
    mydisp.print(data[i]);
  }
}

static void showProbedata(SnifferPacket *snifferPacket) {
  unsigned int frameControl = ((unsigned int)snifferPacket->data[1] << 8) + snifferPacket->data[0];

  uint8_t version      = (frameControl & 0b0000000000000011) >> 0;
  uint8_t frameType    = (frameControl & 0b0000000000001100) >> 2;
  uint8_t frameSubType = (frameControl & 0b0000000011110000) >> 4;
  uint8_t toDS         = (frameControl & 0b0000000100000000) >> 8;
  uint8_t fromDS       = (frameControl & 0b0000001000000000) >> 9;
  uint8_t SSID_length  = snifferPacket->data[25];

  // Only look for probe request packets
  if (frameType != TYPE_MANAGEMENT || frameSubType != SUBTYPE_PROBE_REQUEST) return;

  // Filter out broadcast probes
  if(SSID_length == 0){ return; }

  mydisp.clearScreen();
  mydisp.setCursor(0, 0);

  mydisp.print(F("SSID: "));

  // Save SSID and client MAC address of current probe request
  String SSIDcurrent = getSSID(26, SSID_length, snifferPacket->data);
  mydisp.print(SSIDcurrent.c_str());
  
  mydisp.println();

  mydisp.print(F("RSSI: "));
  mydisp.println(snifferPacket->rx_ctrl.rssi, DEC);

  mydisp.print(F("Ch: "));
  mydisp.println(wifi_get_channel());

  char addr[] = "00:00:00:00:00:00";
  getMAC(addr, snifferPacket->data, 10);
  mydisp.print(F("MAC: "));
  mydisp.println(addr);
}

static void ICACHE_FLASH_ATTR sniffer_probe_callback(uint8_t *buffer, uint16_t length) {
  struct SnifferPacket *snifferPacket = (struct SnifferPacket*) buffer;
  showProbedata(snifferPacket);
}

#define PROBE_CHANNEL_HOP_INTERVAL_MS   2000
static os_timer_t probechannelHop_timer;
void probechannelHop()
{
  byte maxCh;
  if (region_id == 1) { maxCh = 11; }
  else if (region_id == 2) { maxCh = 13; }
  
  // hoping channels 1-14
  uint8 new_channel = wifi_get_channel() + 1;
  if (new_channel > maxCh)
    new_channel = 1;
  wifi_set_channel(new_channel);
}

void ClientBeacons() {
  appmode=1;
  byte count=1;
  byte last=0;
  btnid = 0;
  mydisp.setTextColor(ORANGE);
  mydisp.setTextScale(1);

  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(0);
  delay(10);
  WiFi.disconnect();
  wifi_set_promiscuous_rx_cb(sniffer_probe_callback);
  delay(10);
  wifi_set_channel(1);
  wifi_promiscuous_enable(1);

  os_timer_disarm(&probechannelHop_timer);
  os_timer_setfn(&probechannelHop_timer, (os_timer_func_t *) probechannelHop, NULL);
  os_timer_arm(&probechannelHop_timer, PROBE_CHANNEL_HOP_INTERVAL_MS, 1);
  
  while (1)
  {
    if (btnid == 4) {break;}
    delay(100);
  }
  wifi_promiscuous_enable(0);
  appmode=0;
  btncounter++;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

//String FOX1SSID = "";
//String FOX1MAC = "";
String FOX1SSID = "gee-2g";
String FOX1MAC = "E4:8D:8C:53:BE:BF";
byte FOX1CH;
int FOX1RSSI;
String FOX2SSID = "";
String FOX2MAC = "";
String FOX3SSID = "";
String FOX3MAC = "";

static void inspectFoxHuntdata(SnifferPacket *snifferPacket) {
  unsigned int frameControl = ((unsigned int)snifferPacket->data[1] << 8) + snifferPacket->data[0];

  uint8_t version      = (frameControl & 0b0000000000000011) >> 0;
  uint8_t frameType    = (frameControl & 0b0000000000001100) >> 2;
  uint8_t frameSubType = (frameControl & 0b0000000011110000) >> 4;
  uint8_t toDS         = (frameControl & 0b0000000100000000) >> 8;
  uint8_t fromDS       = (frameControl & 0b0000001000000000) >> 9;
  uint8_t SSID_length  = snifferPacket->data[25];

  //if (frameType != TYPE_MANAGEMENT || frameSubType != SUBTYPE_PROBE_REQUEST) return;

  // Filter out broadcast probes
  if(SSID_length == 0){ return; }
  
  String SSIDcurrent = getSSID(26, SSID_length, snifferPacket->data);
  String tssid = SSIDcurrent.c_str();
  if (tssid == FOX1SSID){
    char addr[] = "00:00:00:00:00:00";
    getMAC(addr, snifferPacket->data, 10);
    String tmac = addr;
    if (tmac == FOX1MAC){
      FOX1CH = wifi_get_channel();
      FOX1RSSI = snifferPacket->rx_ctrl.rssi, DEC;
    }
  }


}

static void ICACHE_FLASH_ATTR sniffer_foxhunt_callback(uint8_t *buffer, uint16_t length) {
  struct SnifferPacket *snifferPacket = (struct SnifferPacket*) buffer;
  inspectFoxHuntdata(snifferPacket);
}

#define FOXHUNT_CHANNEL_HOP_INTERVAL_MS   2000
static os_timer_t foxhuntchannelHop_timer;
void foxhuntchannelHop()
{
  byte maxCh;
  if (region_id == 1) { maxCh = 11; }
  else if (region_id == 2) { maxCh = 13; }
  
  // hoping channels 1-14
  uint8 new_channel = wifi_get_channel() + 1;
  if (new_channel > maxCh)
    new_channel = 1;
  wifi_set_channel(new_channel);
}

void FoxHunt() {
  appmode=1;
  byte count=1;
  byte last=0;
  btnid = 0;
  mydisp.setTextColor(ORANGE);
  mydisp.setTextScale(1);

  if (FOX1MAC == ""){
    mydisp.clearScreen();
    mydisp.setCursor(0, 0);
    mydisp.println(F("No fox definition found"));
    mydisp.println(F("Configure via settings"));
  }
  else{
    wifi_set_opmode(STATION_MODE);
    wifi_promiscuous_enable(0);
    delay(10);
    WiFi.disconnect();
    wifi_set_promiscuous_rx_cb(sniffer_foxhunt_callback);
    delay(10);
    wifi_set_channel(1);
    wifi_promiscuous_enable(1);
  
    os_timer_disarm(&foxhuntchannelHop_timer);
    os_timer_setfn(&foxhuntchannelHop_timer, (os_timer_func_t *) foxhuntchannelHop, NULL);
    os_timer_arm(&foxhuntchannelHop_timer, FOXHUNT_CHANNEL_HOP_INTERVAL_MS, 1);
    
    while (1)
    {
      if (btnid == 4) {break;}

      mydisp.clearScreen();
      mydisp.setCursor(0, 0);
      mydisp.println(F("Seeking out Foxes..."));
      mydisp.print(F("CH: "));
      mydisp.println(wifi_get_channel());

      mydisp.println(F("Fox 1: "));
      if (FOX1CH){
        mydisp.print(F("Last seen on CH: "));  
        mydisp.println(FOX1CH);
        mydisp.print(F("With RSSI: "));  
        mydisp.println(FOX1RSSI);
      }
      
      
      delay(100);
    }
    wifi_promiscuous_enable(0);
    appmode=0;
    btncounter++;  
  }
}

