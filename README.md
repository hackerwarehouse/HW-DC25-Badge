# HW-DC25-Badge

This is the project page for the Hacker Warehouse electronic badge that was made for DEF CON 25.

## Background / timeline of badge development

* This was our first attempt at custom hardware / any electronics project at a larger scale
* 2 hardware revisions, 1 silkscreen revision = 3 prototype runs = 4-5 months
* After final board design was obtained, coding started (all code prior was test code to ensure components were working)
* 2 week code sprint before code needed to be frozen for mass badge flashing
* All boards flashed / all components obtained the week of travel of Las Vegas (one final box arrived the day prior to leaving which gave me some anxiety as well)
* 425 badges were produced 

## Badge Specs

* 96×64 Graphic Full Color OLED Display
* 14x Mini NeoPixel RGB LEDs
* ESP8266EX
* 4 tactile buttons arranged in Joypad configuration
* 8 MB Storage
* Powered via 2 AA batteries

## Badge Functions:

* 802.11 channel activity monitor
* Access point scanner
* LED patterns
* OLED animations
* Personalization
* Other useful network tools

## Release Notes
* There was debate if any/all the mischief tools should be included in the badge by default, include it but have them keycode unlockable, or just not include it in stock firmware. Because we know the people working on/in the Black Hat and DEF CON network, along with wanting to have a usable network without causing more issues, the features were not included. 
  * The continued usage of the word 'locked' in all areas made people assume it was all unlockable during the con. We should have chosen different words for the various areas of inclusion. Sorry about that. 

* Multiple reports during DEF CON on wifi functions not working after x usage. Originally suspected it was the sleep/wakeup or disconnect functions not being triggered properly but that doesn't seem to be it. 
  * As previously mentioned, all the badge software was in a two week sprint which didn't leave much or any time for debugging 

* We originally stated source/binaries would be released after the con but because of wifi bug we were wanting to debug and fix it prior to first public release. It seems like people want the code first so we are releasing it as is with the hopes more eyes on it the better and fixes can be submitted. 




## Compiling

These instructions will get you a copy of the project up and running on your local machine.

### Prerequisites

What things you need to install the software and how to install them

```
1. Arduino / compiler
2. Libraries via package manager
  https://github.com/esp8266/Arduino -> http://arduino.esp8266.com/versions/2.3.0/package_esp8266com_index.json
  https://github.com/adafruit/Adafruit_NeoPixel
  https://github.com/kitesurfer1404/WS2812FX
3. Manually copy the two other library packages to Arduino libraries folder
4. 3.3v serial cable
5. and of course a Hacker Warehouse DC25 Badge
```

### Uploading firmware to badge

1. Press / hold the up button on badge
2. Connect 3.3v serial cable
3. Use arduino compile to upload or use other methods such as esptool or esp8266_flasher (with binary file)
  * 115200


## Authors / Contributors

* **Garrett Gee**
* **Jaycon Systems**

## Acknowledgments

* Hat tip to anyone who's code was used
* All the supporters of this project
* 

