#ifndef HW_MAINMENU_H
#define HW_MAINMENU_H

// required for "const char" and "PROGMEM"
#include <pgmspace.h>

////////////////////////////////////////////////////////////////
// menus - first item is menu title and it does not count toward cnt

static const char PROGMEM itmRoot[] = "HW-DC25 Badge";
static const char PROGMEM itmRootWiFi[] = "WiFi Tools";
static const char PROGMEM itmRootBlinky[] = "Blinky";
static const char PROGMEM itmRootGrafics[] = "Graphics";
static const char PROGMEM itmRootExtras[] = "Extras";
static const char PROGMEM itmRootSettings[] = "Settings";
static const char PROGMEM itmRootAbout[] = "About";
const char * const mnuRoot[] PROGMEM = {
  itmRoot,
  itmRootWiFi,itmRootBlinky,itmRootGrafics,itmRootExtras,itmRootSettings,itmRootAbout
  }; 
const int cntRoot PROGMEM = 6;

static const char PROGMEM itmWiFiTools1[] = "Channel Activity";
static const char PROGMEM itmWiFiTools2[] = "AP Scanner";
const char * const mnuWiFiTools[] PROGMEM = {
  itmRootWiFi,
  itmWiFiTools1,itmWiFiTools2
  };
const int cntWiFiTools PROGMEM = 2;

static const char PROGMEM itmBlinky[] = "Blinky Modes";
static const char PROGMEM itmBlinky1[] = "Random";
static const char PROGMEM itmBlinky2[] = "Larson Scanner";
static const char PROGMEM itmBlinky3[] = "LED Chase";
static const char PROGMEM itmBlinky4[] = "Flashlight";
const char * const mnuBlinky[] PROGMEM = {
  itmBlinky,
  itmBlinky1,itmBlinky2,itmBlinky3,itmBlinky4
  };  
const int cntBlinky PROGMEM = 4;

static const char PROGMEM itmGraphics[] = "Graphic Modes";
static const char PROGMEM itmGraphics1[] = "Benchmark";
static const char PROGMEM itmGraphics2[] = "Rick Roll";
static const char PROGMEM itmGraphics3[] = "Badger";
const char * const mnuGraphics[] PROGMEM = {
  itmGraphics,
  itmGraphics1,itmGraphics2,itmGraphics3
  };  
const int cntGraphics PROGMEM = 3;

static const char PROGMEM itmExtra1[] = "Early Supporters";
static const char PROGMEM itmExtra2[] = "bbb";
const char * const mnuExtra[] PROGMEM = {
  itmRootExtras,
  itmExtra1,itmExtra2
  };  
const int cntExtra PROGMEM = 2;

static const char PROGMEM itmSettings1[] = "Nick/Alias";
static const char PROGMEM itmSettings2[] = "Artwork";
static const char PROGMEM itmSettings3[] = "Brightness";
const char * const mnuSettings[]PROGMEM  = {
  itmRootSettings,
  itmSettings1,itmSettings2,itmSettings3
  }; 
const int cntSettings PROGMEM = 3;

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

static const char PROGMEM itmAbout1[] = "System Version";
static const char PROGMEM itmAbout2[] = "Credits";
const char * const mnuAbout[] PROGMEM = {
  itmRootAbout,
  itmAbout1,itmAbout2
  };  
const int cntAbout PROGMEM = 2;

#endif
