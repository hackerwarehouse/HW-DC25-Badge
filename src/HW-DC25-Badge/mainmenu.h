#ifndef HW_MAINMENU_H
#define HW_MAINMENU_H

// required for "const char" and "PROGMEM"
#include <pgmspace.h>


static const char PROGMEM itmSubmenu1[] = "Channel Activity";
static const char PROGMEM itmItem1[] = "LED Chn Activity";
static const char PROGMEM itmItem2[] = "Logo";

static const char PROGMEM itmSubmenu2[] = "Customization";
static const char PROGMEM itmItem3[] = "Nick/Alias";
static const char PROGMEM itmItem4[] = "Artwork";

static const char PROGMEM itmSubmenu3[] = "AP Scanner";
static const char PROGMEM itmItem5[] = "SSID List";

static const char PROGMEM itmSubmenu4[] = "Blinky";
static const char PROGMEM itmItem6[] = "Random";
static const char PROGMEM itmItem7[] = "Cyclon";
static const char PROGMEM itmItem8[] = "LED Chase";
static const char PROGMEM itmItem9[] = "Flashlight";


static const char PROGMEM itmSubmenu5[] = "Credits ";
static const char PROGMEM itmItem10[] = "Companies";
static const char PROGMEM itmItem11[] = "Websites";


static const char PROGMEM itmSubmenu6[] = "Network Tools";
static const char PROGMEM itmSubmenu7[] = "Shouts";



////////////////////////////////////////////////////////////////
// menus - first item is menu title and it does not count toward cnt

static const char PROGMEM itmRoot[] = "HW-DC25 Badge";
static const char PROGMEM itmRoot1[] = "WiFi Tools";
static const char PROGMEM itmRoot2[] = "Blinky";
static const char PROGMEM itmRoot3[] = "Animations";
static const char PROGMEM itmRoot4[] = "Extras";
static const char PROGMEM itmRoot5[] = "Customize";
static const char PROGMEM itmRoot6[] = "About";
const char * const mnuRoot[] PROGMEM = {
  itmRoot,
  itmRoot1,itmRoot2,itmRoot3,itmRoot4,itmRoot5,itmRoot6
  }; 
const int cntRoot PROGMEM = 6;

static const char PROGMEM itmWiFiTools[] = "WiFi Tools";
static const char PROGMEM itmWiFiTools1[] = "Channel Activity";
static const char PROGMEM itmWiFiTools2[] = "AP Scanner";
const char * const mnuWiFiTools[] PROGMEM = {
  itmWiFiTools,
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

static const char PROGMEM itmAnimation[] = "Animation Modes";
static const char PROGMEM itmAnimation1[] = "Eyes";
static const char PROGMEM itmAnimation2[] = "Rick Roll";
static const char PROGMEM itmAnimation3[] = "Badger";
const char * const mnuAnimation[] PROGMEM = {
  itmAnimation,
  itmAnimation1,itmAnimation2,itmAnimation3
  };  
const int cntAnimation PROGMEM = 3;

static const char PROGMEM itmExtra[] = "Extras";
static const char PROGMEM itmExtra1[] = "aaa";
static const char PROGMEM itmExtra2[] = "bbb";
const char * const mnuExtra[] PROGMEM = {
  itmExtra,
  itmExtra1,itmExtra2
  };  
const int cntExtra PROGMEM = 2;

static const char PROGMEM itmCustomize[] = "Customization";
static const char PROGMEM itmCustomize1[] = "Nick/Alias";
static const char PROGMEM itmCustomize2[] = "Artwork";
const char * const mnuCustomize[]PROGMEM  = {
  itmCustomize,
  itmCustomize1,itmCustomize2
  }; 
const int cntCustomize PROGMEM = 2;

static const char PROGMEM itmAbout[] = "About";
static const char PROGMEM itmAbout1[] = "System Version";
static const char PROGMEM itmAbout2[] = "Credits";
const char * const mnuAbout[] PROGMEM = {
  itmAbout,
  itmAbout1,itmAbout2
  };  
const int cntAbout PROGMEM = 2;

#endif
