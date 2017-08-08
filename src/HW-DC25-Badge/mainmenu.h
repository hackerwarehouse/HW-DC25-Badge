#ifndef HW_MAINMENU_H
#define HW_MAINMENU_H

// required for "const char" and "PROGMEM"
#include <pgmspace.h>

// mnu* note - first item is menu title and it does not count toward cnt

static const char PROGMEM itmRoot[] = "HW-DC25 Badge";
static const char PROGMEM itmRootWiFi[] = "WiFi Tools";
static const char PROGMEM itmRootBlinky[] = "Blinky";
static const char PROGMEM itmRootGrafics[] = "Graphics";
static const char PROGMEM itmRootClient[] = "Client Tools";
static const char PROGMEM itmRootMischief[] = "Mischief Tools";
static const char PROGMEM itmRootExtras[] = "Extras";
static const char PROGMEM itmRootSettings[] = "Settings";
static const char PROGMEM itmRootAbout[] = "About";
const char * const mnuRoot[] PROGMEM = {
  itmRoot,
  itmRootWiFi,itmRootBlinky,itmRootGrafics,itmRootMischief,itmRootExtras,itmRootSettings,itmRootAbout
  }; 
const int cntRoot PROGMEM = 7;

static const char PROGMEM itmWiFiTools1[] = "Channel Usage";
static const char PROGMEM itmWiFiTools2[] = "AP Scanner";
static const char PROGMEM itmWiFiTools3[] = "Packet Monitor";
static const char PROGMEM itmWiFiTools4[] = "Deauth Detect";
static const char PROGMEM itmWiFiTools5[] = "Deauth Monitor";
static const char PROGMEM itmWiFiTools6[] = "Fox Hunt";
static const char PROGMEM itmWiFiTools7[] = "Open AP Nearby?";
static const char PROGMEM itmWiFiTools8[] = "Client Beacons";
const char * const mnuWiFiTools[] PROGMEM = {
  itmRootWiFi,
  itmWiFiTools1,itmWiFiTools2,itmWiFiTools3,itmWiFiTools4,itmWiFiTools5,itmWiFiTools6,itmWiFiTools7,itmWiFiTools8
  };
const int cntWiFiTools PROGMEM = 8;

static const char PROGMEM itmBlinky[] = "Blinky Modes";
static const char PROGMEM itmBlinky0[] = "Favorites";
static const char PROGMEM itmBlinky1[] = "Flashlight";
static const char PROGMEM itmBlinky2[] = "Full Demo";
static const char PROGMEM itmBlinky3[] = "Blink";
static const char PROGMEM itmBlinky4[] = "Breath";
static const char PROGMEM itmBlinky5[] = "Color Wipe";
static const char PROGMEM itmBlinky6[] = "Color Wipe Rndm";
static const char PROGMEM itmBlinky7[] = "Random Color";
static const char PROGMEM itmBlinky8[] = "Single Dynamic";
static const char PROGMEM itmBlinky9[] = "Multi Dynamic";
static const char PROGMEM itmBlinky10[] = "Rainbow";
static const char PROGMEM itmBlinky11[] = "Rainbow Cycle";
static const char PROGMEM itmBlinky12[] = "Scan";
static const char PROGMEM itmBlinky13[] = "Dual Scan";
static const char PROGMEM itmBlinky14[] = "Fade";
static const char PROGMEM itmBlinky15[] = "Theater Chase";
static const char PROGMEM itmBlinky16[] = "Theater Chase Rnbw";
static const char PROGMEM itmBlinky17[] = "Running Lights";
static const char PROGMEM itmBlinky18[] = "Twinkle";
static const char PROGMEM itmBlinky19[] = "Twinkle Random";
static const char PROGMEM itmBlinky20[] = "Twinkle Fade";
static const char PROGMEM itmBlinky21[] = "Twinkle Fade Rndm";
static const char PROGMEM itmBlinky22[] = "Sparkle";
static const char PROGMEM itmBlinky23[] = "Flash Sparkle";
static const char PROGMEM itmBlinky24[] = "Hyper Sparkle";
static const char PROGMEM itmBlinky25[] = "Strobe";
static const char PROGMEM itmBlinky26[] = "Strobe Rainbow";
static const char PROGMEM itmBlinky27[] = "Multi Strobe";
static const char PROGMEM itmBlinky28[] = "Blink Rainbow";
static const char PROGMEM itmBlinky29[] = "Chase White";
static const char PROGMEM itmBlinky30[] = "Chase Color";
static const char PROGMEM itmBlinky31[] = "Chase Random";
static const char PROGMEM itmBlinky32[] = "Chase Rainbow";
static const char PROGMEM itmBlinky33[] = "Chase Flash";
static const char PROGMEM itmBlinky34[] = "Chase Flash Rndm";
static const char PROGMEM itmBlinky35[] = "Chase Rnbw White";
static const char PROGMEM itmBlinky36[] = "Chase Blackout";
static const char PROGMEM itmBlinky37[] = "Chase Blckot Rnbw";
static const char PROGMEM itmBlinky38[] = "Color Sweep Rndm";
static const char PROGMEM itmBlinky39[] = "Running Color";
static const char PROGMEM itmBlinky40[] = "Running Red Blue";
static const char PROGMEM itmBlinky41[] = "Running Random";
static const char PROGMEM itmBlinky42[] = "Larson Scanner";
static const char PROGMEM itmBlinky43[] = "Comet";
static const char PROGMEM itmBlinky44[] = "Fireworks";
static const char PROGMEM itmBlinky45[] = "Fireworks Random";
static const char PROGMEM itmBlinky46[] = "Merry xmas";
static const char PROGMEM itmBlinky47[] = "Fire Flicker";
static const char PROGMEM itmBlinky48[] = "Fire Flicker Soft";
const char * const mnuBlinky[] PROGMEM = {
  itmBlinky,
  itmBlinky0,itmBlinky1,itmBlinky2,itmBlinky3,itmBlinky4,itmBlinky5,itmBlinky6,itmBlinky7,itmBlinky8,itmBlinky9,
  itmBlinky10,itmBlinky11,itmBlinky12,itmBlinky13,itmBlinky14,itmBlinky15,itmBlinky16,itmBlinky17,itmBlinky18,itmBlinky19,
  itmBlinky20,itmBlinky21,itmBlinky22,itmBlinky23,itmBlinky24,itmBlinky25,itmBlinky26,itmBlinky27,itmBlinky28,itmBlinky29,
  itmBlinky30,itmBlinky31,itmBlinky32,itmBlinky33,itmBlinky34,itmBlinky35,itmBlinky36,itmBlinky37,itmBlinky38,itmBlinky39,
  itmBlinky40,itmBlinky41,itmBlinky42,itmBlinky43,itmBlinky44,itmBlinky45,itmBlinky46,itmBlinky47,itmBlinky48
  };  
const int cntBlinky PROGMEM = 49;

static const char PROGMEM itmGraphics[] = "Graphic Modes";
static const char PROGMEM itmGraphics1[] = "Artwork";
static const char PROGMEM itmGraphics2[] = "3d Cube Demo";
static const char PROGMEM itmGraphics3[] = "Clock Demo";
static const char PROGMEM itmGraphics4[] = "Gauge Demo";
static const char PROGMEM itmGraphics5[] = "Benchmark";
const char * const mnuGraphics[] PROGMEM = {
  itmGraphics,
  itmGraphics1,itmGraphics2,itmGraphics3,itmGraphics4,itmGraphics5
  };  
const int cntGraphics PROGMEM = 5;

static const char PROGMEM itmClient1[] = "Connect to an AP";
static const char PROGMEM itmClient2[] = "";
static const char PROGMEM itmClient3[] = "";
static const char PROGMEM itmClient4[] = "Disconnect from AP";
const char * const mnuClient[] PROGMEM = {
  itmRootClient,
  itmClient1,itmClient2,itmClient3,itmClient4
  };  
const int cntClient PROGMEM = 4;

static const char PROGMEM itmMischief1[] = "Deauther";
static const char PROGMEM itmMischief2[] = "Karma/Pineapple lite";
static const char PROGMEM itmMischief3[] = "Broadcast Packets";
static const char PROGMEM itmMischief4[] = "SSID Rickroll";
static const char PROGMEM itmMischief5[] = "SSID Havoc";
const char * const mnuMischief[] PROGMEM = {
  itmRootMischief,
  itmMischief1,itmMischief2,itmMischief3,itmMischief4,itmMischief5
  };  
const int cntMischief PROGMEM = 5;

static const char PROGMEM itmExtra1[] = "WiFi off";
static const char PROGMEM itmExtra2[] = "Snake Game";
static const char PROGMEM itmExtra3[] = "Button Echo";
static const char PROGMEM itmExtra4[] = "Keyboard Demo";
const char * const mnuExtra[] PROGMEM = {
  itmRootExtras,
  itmExtra1,itmExtra2,itmExtra3,itmExtra4
  };  
const int cntExtra PROGMEM = 4;

static const char PROGMEM itmSettings1[] = "Brightness";
static const char PROGMEM itmSettings2[] = "Region";
static const char PROGMEM itmSettings3[] = "Other via WiFi";
static const char PROGMEM itmSettings4[] = "";
const char * const mnuSettings[]PROGMEM  = {
  itmRootSettings,
  itmSettings1,itmSettings2,itmSettings3
  }; 
const int cntSettings PROGMEM = 2;

static const char PROGMEM itmBrightness[] = "Brightness Setting";
static const char PROGMEM itmBrightness1[] = "Uber Low";
static const char PROGMEM itmBrightness2[] = "Low";
static const char PROGMEM itmBrightness3[] = "Medium";
static const char PROGMEM itmBrightness4[] = "High";
static const char PROGMEM itmBrightness5[] = "Too Bright!";
const char * const mnuBrightness[] PROGMEM = {
  itmBrightness,
  itmBrightness1,itmBrightness2,itmBrightness3,itmBrightness4,itmBrightness5
  };  
const int cntBrightness PROGMEM = 5;

static const char PROGMEM itmRegion[] = "Region Setting";
static const char PROGMEM itmRegion1[] = "United States";
static const char PROGMEM itmRegion2[] = "European Union";
const char * const mnuRegion[] PROGMEM = {
  itmRegion,
  itmRegion1,itmRegion2
  };  
const int cntRegion PROGMEM = 2;

static const char PROGMEM itmAbout1[] = "System Info";
static const char PROGMEM itmAbout2[] = "ipconfig";
static const char PROGMEM itmAbout3[] = "Credits";
static const char PROGMEM itmAbout4[] = "Early Supporters";
static const char PROGMEM itmAbout5[] = "Debug";
const char * const mnuAbout[] PROGMEM = {
  itmRootAbout,
  itmAbout1,itmAbout2,itmAbout3,itmAbout4,itmAbout5
  };  
const int cntAbout PROGMEM = 5;

#endif
