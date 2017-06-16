#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SSD_13XX.h>
#include <SPI.h>
#include <qMenuDisplay.h>
#include <qMenuSystem.h>

#include "TestMenu.h"
#include "_images/godai_1.c"  //New image for artwork feature 
#include "_images/blank.c"  //New image for artwork feature 
#include "_images/defcon.c" //New image for artwork feature 
#include "_images/badge.c"
#include "_images/hacker.c"
#include "_images/godai.c"
#include "_images/js.c"
#include "_images/garret.c"
#include "_fonts/defaultFont.c"

#include "apscanner.h"
#include "blinky.h"
#include "channelactivity.h"
#include "core.h"

#define NUM_SAMPLES 10  //Number of readings for battery voltage


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
SSD_13XX mydisp(_CS, _DC);
qMenuSystem menu=qMenuSystem(&mydisp);

const byte left = 15; 
const byte down = 10; 
const byte right = 2; 
const byte up = 0;  

volatile byte counter = 0;
volatile byte id  = 0;
volatile byte img = 0;
volatile byte region_id = 1; //1->US, 2->EU, 3->JP  US default

long debouncing_time = 250;
unsigned long last_micros = 0;

//Variables for Batt voltage readings
float voltage = 0.0;
float real_voltage = 0.0;
float threshold = 2.7;  //Min operating voltage for booster (used for screen). Will test with slightly smaller voltages to see if it can handle it and extend badge life.
int sum = 0.0;
int sample_count = 0;
const int battery = A0;

int h, w, buffidx;  //Variables for large images 

//Add tasks as needed 
enum Tasks {Null,
            Random,
            Cyclon,
            Chase,
            Flashlight,
            Scan,
            Channel,
            Artwork
            };    

Tasks CurrentTask = Null;

void turn_off(){
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
    pixels.show();
  }  
}

void UP(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
  counter ++;
  id = 3;
  last_micros = micros();
  }
}

void DOWN(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
  counter ++;
  id = 2;
  last_micros = micros();
  }
}

void RIGHT(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
  counter ++;
  id = 1;
  last_micros = micros();
  }
}

void LEFT(){
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
  counter ++;
  id = 4;
  last_micros = micros();
  }
}

void display_image(){
  int image = EEPROM.read(0);   //Read saved image and display it
  switch(image){
    
  case 1:
  h = 63;
  w = 73;
  buffidx = 0;
  mydisp.clearScreen();
  //mydisp.drawImage(11,0,&badge);
  for (int row=0; row<h; row++) {
    for (int col=0; col<w; col++) { 
      mydisp.drawPixel(col+11, row, pgm_read_word(image_data_badge + buffidx));
      buffidx++;
    } 
  }
  break;
  
  case 2:
  h = 63;
  w = 63;
  buffidx = 0;
  mydisp.clearScreen();
  //mydisp.drawImage(17,0,&godai_1);
  for (int row=0; row<h; row++) {
    for (int col=0; col<w; col++) { 
      mydisp.drawPixel(col+17, row, pgm_read_word(image_data_godai_1 + buffidx));
      buffidx++;
    } 
  }
  break;
  
  case 3:
  h = 25;
  w = 96;
  buffidx = 0;
  mydisp.clearScreen(); 
  //mydisp.drawImage(0,20,&defcon);
  for (int row=0; row<h; row++) {
    for (int col=0; col<w; col++) { 
      mydisp.drawPixel(col, row+20, pgm_read_word(image_data_defcon + buffidx));
      buffidx++;
    } 
  }
  break;
  
  case 4: 
  h = 64;
  w = 96;
  buffidx = 0;
  mydisp.clearScreen();
  //mydisp.drawImage(0,0,&blank);
  for (int row=0; row<h; row++) {
    for (int col=0; col<w; col++) { 
      mydisp.drawPixel(col+17, row, pgm_read_word(image_data_blank+ buffidx));
      buffidx++;
    } 
  }
  break;
 }
}

//Function used to map the analog input voltage (1V - Voltage Divider in circuit) to max battery voltage (3V - 2xAA)
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
} 

void setup()
{  
  EEPROM.begin(512);  //Initialize eeprom 
  
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  pinMode(left, INPUT); //External pulldown resistor (required for programming)

  attachInterrupt(digitalPinToInterrupt(up), UP, FALLING);
  attachInterrupt(digitalPinToInterrupt(down), DOWN, FALLING);
  attachInterrupt(digitalPinToInterrupt(right), RIGHT, FALLING);
  attachInterrupt(digitalPinToInterrupt(left), LEFT, RISING);

  menu.InitMenu((const char **)mnuRoot,cntRoot,1);
  pixels.begin();
  turn_off();
  Serial.begin(115200);
}

void loop()
{
////Check battery during operation /////

  while(sample_count < NUM_SAMPLES){  //Average battery analog readings  
    sum+=analogRead(battery);
    sample_count++;
    delay(1);
  }

  voltage = ((float)sum / (float)NUM_SAMPLES * 1.08) / 1024.0; //Get voltage (0-1V)
  real_voltage = mapfloat(voltage, 0.0, 1.0, 0.0, 3.0); //Get real voltage (0-3V) 
  sample_count =0;
  sum =0;

  if (real_voltage< threshold){ //Check if batteries are above threshold 
    mydisp.clearScreen();
    mydisp.setTextColor(RED);
    mydisp.setTextScale(2);
    mydisp.setCursor(15, 0);
    mydisp.println("Change");
    mydisp.println("");
    mydisp.print("Batteries!!!!");
    delay(3000);
    mydisp.clearScreen();
    ESP.deepSleep(0); //Put device in sleep mode forever until batteries have been changed   
  }

  int keycode=0;
  int clickedItem=0;

  if (CurrentTask == Random){
    LED1();
    if (counter > 0){
      turn_off();   
      CurrentTask = Null;
    }  
  }

  else if (CurrentTask == Cyclon){
    LED2();
    if (counter > 0){
      turn_off();   
      CurrentTask = Null;
    }
  }

  else if (CurrentTask == Chase){
    LED3();
    if (counter > 0){
      turn_off();   
      CurrentTask = Null;
    }
  }

  else if (CurrentTask == Flashlight){
    LED4();
    if (counter > 0){
      turn_off();  
      CurrentTask = Null;
    }
  }
  
  else if (CurrentTask == Scan){
    Scanner();
    CurrentTask = Null;
  }

  else if (CurrentTask == Channel){
    Channel_Activity();
    turn_off();
    mydisp.setFont(&defaultFont);
    CurrentTask = Null;   
  }

  else if  (CurrentTask == Artwork && counter > 0){
    switch(id){
      case 1:
      counter--;
      EEPROM.write(0,img);  //Save selected image in eeprom
      EEPROM.commit();
      menu.MessageBox("Saving...");
      delay(1000);
      CurrentTask = Null;
      menu.InitMenu((const char ** )mnuSubmenu2,cntSubmenu2,1); //Return to menu 
      break;
      
      case 2:
      counter--;  
      img --;
      switch(img){
        case 1:
        h = 63;
        w = 73;
        buffidx = 0;
        mydisp.clearScreen();
        //mydisp.drawImage(11,0,&badge);
        for (int row=0; row<h; row++) {
          for (int col=0; col<w; col++) { 
            mydisp.drawPixel(col+11, row, pgm_read_word(image_data_badge + buffidx));
            buffidx++;
          } 
        }
        break;
  
        case 2:
        h = 63;
        w = 63;
        buffidx = 0;
        mydisp.clearScreen();
        //mydisp.drawImage(17,0,&godai_1);
        for (int row=0; row<h; row++) {
          for (int col=0; col<w; col++) { 
            mydisp.drawPixel(col+17, row, pgm_read_word(image_data_godai_1 + buffidx));
            buffidx++;
          } 
        }
        break;
  
        case 3:
        h = 25;
        w = 96;
        buffidx = 0;
        mydisp.clearScreen(); 
        //mydisp.drawImage(0,20,&defcon);
        for (int row=0; row<h; row++) {
          for (int col=0; col<w; col++) { 
            mydisp.drawPixel(col, row+20, pgm_read_word(image_data_defcon + buffidx));
            buffidx++;
          } 
        }
        break;
  
        case 4: //Blank
        mydisp.clearScreen();
        menu.MessageBox("No Artwork");
        break;
        
        //Wrap around case
  
        case 0: //Blank 
        mydisp.clearScreen();
        img = 4;  //Reset image id 
        menu.MessageBox("No Artwork");
        break;
      }
      break;
      
      case 3:
      counter--;
      img ++;
      switch(img){
        case 1:
        h = 63;
        w = 73;
        buffidx = 0;
        mydisp.clearScreen();
        //mydisp.drawImage(11,0,&badge);
        for (int row=0; row<h; row++) {
          for (int col=0; col<w; col++) { 
            mydisp.drawPixel(col+11, row, pgm_read_word(image_data_badge + buffidx));
            buffidx++;
          } 
        }
        break;
  
        case 2:
        h = 63;
        w = 63;
        buffidx = 0;
        mydisp.clearScreen();
        //mydisp.drawImage(17,0,&godai_1);
        for (int row=0; row<h; row++) {
          for (int col=0; col<w; col++) { 
            mydisp.drawPixel(col+17, row, pgm_read_word(image_data_godai_1 + buffidx));
            buffidx++;
          } 
        }
        break;
  
        case 3:
        h = 25;
        w = 96;
        buffidx = 0;
        mydisp.clearScreen(); 
        //mydisp.drawImage(0,20,&defcon);
        for (int row=0; row<h; row++) {
          for (int col=0; col<w; col++) { 
            mydisp.drawPixel(col, row+20, pgm_read_word(image_data_defcon + buffidx));
            buffidx++;
          } 
        }
        break;
  
        case 4:
        mydisp.clearScreen();
        menu.MessageBox("No Artwork");
        break;
  
        //Wraparound case
  
        case 5:
        h = 63;
        w = 73;
        buffidx = 0;
        img = 1;
        mydisp.clearScreen();
        //mydisp.drawImage(11,0,&badge);
        for (int row=0; row<h; row++) {
          for (int col=0; col<w; col++) { 
            mydisp.drawPixel(col+11, row, pgm_read_word(image_data_badge + buffidx));
            buffidx++;
          } 
        }
        break;
      }
      break;
      
      case 4:
      CurrentTask = Null;
      menu.InitMenu((const char ** )mnuRoot,cntRoot,2);
      counter--;
      break;
   }
 }

  else if (counter > 0 && CurrentTask == Null)
  { 
    switch(id)
    {
      case 1:       
        clickedItem=menu.ProcessMenu(ACTION_SELECT);
        counter--;
        break;
      case 2:
        menu.ProcessMenu(ACTION_DOWN);
        counter--;
        break;      
      case 3:
        menu.ProcessMenu(ACTION_UP);
        counter--;
        break;
      case 4:
        clickedItem=menu.ProcessMenu(ACTION_BACK); 
        counter--;
        break;
    }
  }
  
  if (clickedItem > 0)
  {
    // Logic for Root menu
    if (menu.CurrentMenu==mnuRoot)
      switch (clickedItem)
      {
        case 1:
          menu.InitMenu((const char ** )mnuSubmenu1,cntSubmenu1,1);
          break;
        case 2:
          menu.InitMenu((const char ** )mnuSubmenu2,cntSubmenu2,1);
          break;
        case 3:
          menu.InitMenu((const char ** )mnuSubmenu3,cntSubmenu3,1);
          break;
        case 4:
          menu.InitMenu((const char ** )mnuSubmenu4,cntSubmenu4,1);
          break; 
         case 5:
          menu.InitMenu((const char ** )mnuSubmenu5,cntSubmenu5,1);
          break;
         case 6:
          menu.InitMenu((const char ** )mnuSubmenu6,cntSubmenu6,1);
          break;
         case 7:
          menu.InitMenu((const char ** )mnuSubmenu7,cntSubmenu7,1);
          break;
      }
      
    // Logic for Submenu 1
    else if (menu.CurrentMenu==mnuSubmenu1)
      switch (clickedItem)
      {
        case 1:
        menu.MessageBox("Scanning...");
        CurrentTask = Channel;
        break;
        
        case 2:
        display_image();
        break;
      }
      
    // Logic for Submenu 2
    else if (menu.CurrentMenu==mnuSubmenu2)
      switch (clickedItem)
      {
        case 1:
        CurrentTask = Null;
        break;
        
        case 2:
        h = 63;
        w = 73;
        buffidx = 0;
        
        mydisp.clearScreen();
        menu.MessageBox("Select Artwork");
        delay(1000);
        mydisp.clearScreen(); 
        /*
        This method uses a lot of dynamic memory
     
        mydisp.drawImage(11, 0, &badge); //Make HW artwork always show first  
        img = 1;
        CurrentTask = Artwork;
        

        This method puts image in EEPROM, saving dynamic memory, but slower*/
        for (int row=0; row<h; row++) {
          for (int col=0; col<w; col++) { 
            mydisp.drawPixel(col+11, row, pgm_read_word(image_data_badge + buffidx));
            buffidx++;
          } 
        }
        img = 1;
        CurrentTask = Artwork; 
        break;

        case 3: 
        menu.InitMenu((const char ** )mnuRegion,cntmnuRegion,1);
        menu.CurrentMenu==mnuRegion;
        break;
      }
      
    // Logic for Submenu 3
    else if (menu.CurrentMenu==mnuSubmenu3)
      switch (clickedItem)
      {
        case 1:
          menu.MessageBox("Scanning...");
          CurrentTask = Scan;
          break;
      }
      
    // Logic for Submenu 4
    else if (menu.CurrentMenu==mnuSubmenu4)
      switch (clickedItem)
      {
        case 1:
          menu.MessageBox("");
          CurrentTask=Random;
          break;
        case 2:
          menu.MessageBox("");
          CurrentTask=Cyclon;  
          break;
         case 3:
          menu.MessageBox("");
          CurrentTask=Chase;  
          break;
         case 4:
          menu.MessageBox("");
          CurrentTask=Flashlight;  
          break;
      }

      // Logic for Submenu 5
    else if (menu.CurrentMenu==mnuSubmenu5)
      switch (clickedItem)
      {
        ///This images can be stored in dynamic memory. No need to put them in PROGMEM
        case 1:
        mydisp.clearScreen();
        mydisp.setRotation(0);
        mydisp.setTextColor(GREENYELLOW);
        mydisp.drawImage(0, 0, &js);
        mydisp.setCursor(17, 4);
        mydisp.print("Jaycon Systems");
        mydisp.drawImage(1, 16, &hacker);
        mydisp.setCursor(17, 20);
        mydisp.print("Hacker Warehouse");
        mydisp.drawImage(1, 32, &godai);
        mydisp.setCursor(17, 36);
        mydisp.print("Godai Group");
        mydisp.drawImage(1, 48, &garret);
        mydisp.setCursor(17, 52);
        mydisp.print("Garrett Gee 2");
        
        break;
        case 2:
        mydisp.clearScreen();
        mydisp.setRotation(0);
        mydisp.setTextColor(CYAN);
        mydisp.setCursor(0, 4);
        mydisp.print("jayconsystems.com");
        mydisp.drawLine(0, 12, 86, 12, CYAN);
        mydisp.setCursor(0, 20);
        mydisp.print("hackerwarehouse.com");
        mydisp.drawLine(0, 28, 93, 28, CYAN);
        mydisp.setCursor(0, 36);
        mydisp.print("godaigroup.net");
        mydisp.drawLine(0, 44, 60, 44, CYAN);
        mydisp.setCursor(0, 52);
        mydisp.print("garrettgee.com");
        mydisp.drawLine(0, 60, 64, 60, CYAN);
      }

      // Logic for Submenu 6
    else if (menu.CurrentMenu==mnuSubmenu6)
      switch (clickedItem)
      {
        case 1:
        case 2:
        case 3:
          break;
      }

      // Logic for Submenu 7
    else if (menu.CurrentMenu==mnuSubmenu7)
      switch (clickedItem)
      {
        case 1:
        case 2:
        case 3:
          break;
      }
    //logic for Region Submenu
    else if (menu.CurrentMenu==mnuRegion)
      switch (clickedItem)
      {
        case 1:
        region_id = 1;  //US
        EEPROM.write(1,region_id);  
        EEPROM.commit();
        menu.MessageBox("Region Set");
        delay(1000);
        menu.InitMenu((const char ** )mnuRegion,cntmnuRegion,1);
        break;
        
        case 2:
        region_id = 2;  //EU
        EEPROM.write(1,region_id);  
        EEPROM.commit();
        menu.MessageBox("Region Set");
        delay(1000);
        menu.InitMenu((const char ** )mnuRegion,cntmnuRegion,2);
        break;      
      }
  } 
  
  else if(clickedItem == -1){ // Return Logic
    // Logic for Submenu 1
    if (menu.CurrentMenu==mnuRoot)
      {
        //Do Nothing
      }

    // Logic for Submenu 1
    else if (menu.CurrentMenu==mnuSubmenu1)   
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,1);
      }

    // Logic for Submenu 2
    else if (menu.CurrentMenu==mnuSubmenu2)
      {     
        menu.InitMenu((const char ** )mnuRoot,cntRoot,2);
      }

    // Logic for Submenu 3
    else if (menu.CurrentMenu==mnuSubmenu3)
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,3);
      }

    // Logic for Submenu 4
    else if (menu.CurrentMenu==mnuSubmenu4)
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,4);
      }

    // Logic for Submenu 5
    else if (menu.CurrentMenu==mnuSubmenu5)
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,5);
      }

     // Logic for Submenu 6
    else if (menu.CurrentMenu==mnuSubmenu6)
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,6);
      }

    // Logic for Submenu 7
    else if (menu.CurrentMenu==mnuSubmenu7)
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,7);
      }

    else if (menu.CurrentMenu==(const char **)"SSID List")
      {
        menu.InitMenu((const char ** )mnuRoot,cntRoot,3);
      }

    else if (menu.CurrentMenu==mnuRegion){
       menu.InitMenu((const char ** )mnuSubmenu2,cntSubmenu2,3);
    }

   }
 }





