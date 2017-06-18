#ifndef HW_MAINMENU_H
#define HW_MAINMENU_H

// required for "const char" and "PROGMEM"
#include <pgmspace.h>

////////////////////////////////////////////////////////////////
// menus - first item is menu title and it does not count toward cnt

 const char * const mnuRoot[] PROGMEM = {
  "HW-DC25 Badge",
  "WiFi Tools","Blinky","Animations", "Extras", "Customize", "About"
  }; 
 const int cntRoot PROGMEM = 6;

 const char * const mnuWiFiTools[] PROGMEM = {
  "WiFi Tools",
  "Channel Activity","AP Scanner"
  };
 const int cntWiFiTools PROGMEM = 2;

 const char * const mnuBlinky[] PROGMEM = {
  "Blinky Modes",
  "Random", "Larson Scanner", "LED Chase", "Flashlight"
  };  
 const int cntBlinky PROGMEM = 4;

 const char * const mnuAnimation[] PROGMEM = {
  "Animation Modes",
  "Toaster","Rick Roll","Badger"
  };  
 const int cntAnimation PROGMEM = 3;

 const char * const mnuExtra[] PROGMEM = {
  "Extras",
  "aaa", "bbb"
  };  
 const int cntExtra PROGMEM = 2;

 const char * const mnuCustomize[]PROGMEM  = {
  "Customization",
  "Nick/Alias", "Artwork"
  }; 
 const int cntCustomize PROGMEM = 2;

 const char * const mnuAbout[] PROGMEM = {
  "About",
  "System Version","Credits"
  };  
 const int cntAbout PROGMEM = 2;

#endif
