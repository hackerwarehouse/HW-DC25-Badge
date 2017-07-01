#include <EEPROM.h>
#include <SPI.h>

#include <SSD_13XX.h>

#include "core.h"

extern SSD_13XX mydisp;
extern byte appmode;
extern byte btncounter;
extern volatile byte btnid;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "_images/badge.c"
#include "_images/godai_1.c"
#include "_images/defcon.c"
#include "_images/hackerwarehouse.c"

void DisplayArtwork(byte img){
  int h, w, buffidx;
  int row, col;
  switch(img)
  {
   case 1:
      h = 31;
      w = 95;
      buffidx = 0;
      mydisp.clearScreen(); 
      for (row=0; row<h; row++) {
        for (col=0; col<w; col++) { 
          mydisp.drawPixel(col, row+20, pgm_read_word(image_data_hackerwarehouse + buffidx));
          buffidx++;
        } 
      }
      break;
   case 2:
      h = 25;
      w = 96;
      buffidx = 0;
      mydisp.clearScreen(); 
      for (row=0; row<h; row++) {
        for (col=0; col<w; col++) { 
          mydisp.drawPixel(col, row+20, pgm_read_word(image_data_defcon + buffidx));
          buffidx++;
        } 
      }
      break;
   case 3:
      h = 63;
      w = 73;
      buffidx = 0;
      mydisp.clearScreen();
      for (row=0; row<h; row++) {
        for (col=0; col<w; col++) { 
          mydisp.drawPixel(col+11, row, pgm_read_word(image_data_badge + buffidx));
          buffidx++;
        } 
      }
      break;
   case 4:
      h = 63;
      w = 63;
      buffidx = 0;
      mydisp.clearScreen();
      for (row=0; row<h; row++) {
        for (col=0; col<w; col++) { 
          mydisp.drawPixel(col+17, row, pgm_read_word(image_data_godai_1 + buffidx));
          buffidx++;
        } 
      }
      break;
  }
}

void SelectArtwork(){
  appmode=1;
  byte count=1;
  byte last=0;
  btnid = 0;
  while (1)
  {
    if (btnid == 3){ 
      if (count == 4) {count = 1; } 
      else {count++;}
      btnid = 0;
    }
    else if (btnid == 2){ 
      if (count == 1) {count = 4; } 
      else {count--;}
      btnid = 0;
    }
    else if (btnid == 1){
      EEPROM.write(ARTWORK_ADDR,count);
      EEPROM.commit();
      mydisp.clearScreen();
      mydisp.setCursor(0, 0);
      mydisp.setTextColor(GREENYELLOW);
      mydisp.setTextScale(3);
      mydisp.println(F("Saved"));
      delay(1000);
      btnid = 4;
      break;
    }
    else if (btnid == 4) {break;}

    if (count != last)
    {
      last=count;
      DisplayArtwork(last); 
    }
    delay(100);
  }
  appmode=0;
  btncounter++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long testFillScreen() {
  unsigned long start = micros();
  mydisp.clearScreen();
  mydisp.fillScreen(RED);
  mydisp.fillScreen(GREEN);
  mydisp.fillScreen(BLUE);
  mydisp.clearScreen();
  return micros() - start;
}

unsigned long testText() {
  mydisp.clearScreen();
  unsigned long start = micros();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(1);
  mydisp.println("Hello World!");
  mydisp.setTextColor(YELLOW);
  mydisp.setTextScale(2);
  mydisp.println(1234.56);
  mydisp.setTextColor(RED);
  mydisp.setTextScale(3);
  mydisp.println(0xDEAD, HEX);
  mydisp.println();
  mydisp.setTextColor(GREEN);
  mydisp.setTextScale(4);
  mydisp.println("Hello");
  return micros() - start;


}

unsigned long testText2() {
  mydisp.clearScreen();
  unsigned long start = micros();
  mydisp.setCursor(0, 0);
  mydisp.setTextColor(WHITE);
  mydisp.setTextScale(2);
  mydisp.println(F("I implore thee,"));
  mydisp.setTextScale(1);
  mydisp.println(F("my foonting turlingdromes."));
  mydisp.println(F("And hooptiously drangle me"));
  mydisp.println(F("with crinkly bindlewurdles,"));
  mydisp.println(F("Or I will rend thee"));
  mydisp.println(F("in the gobberwarts"));
  mydisp.println(F("with my blurglecruncheon,"));
  mydisp.println(F("see if I don't!"));
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = mydisp.width(),
                h = mydisp.height();

  mydisp.clearScreen();

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) mydisp.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for (y2 = 0; y2 < h; y2 += 6) mydisp.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  mydisp.clearScreen();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) mydisp.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for (y2 = 0; y2 < h; y2 += 6) mydisp.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  mydisp.clearScreen();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) mydisp.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for (y2 = 0; y2 < h; y2 += 6) mydisp.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  mydisp.clearScreen();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) mydisp.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for (y2 = 0; y2 < h; y2 += 6) mydisp.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = mydisp.width(), h = mydisp.height();

  mydisp.clearScreen();
  start = micros();
  for (y = 0; y < h; y += 5) mydisp.drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5) mydisp.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = mydisp.width()  / 2,
                cy = mydisp.height() / 2;

  mydisp.clearScreen();
  n     = min(mydisp.width(), mydisp.height());
  start = micros();
  for (i = 2; i < n; i += 6) {
    i2 = i / 2;
    mydisp.drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = (mydisp.width()  / 2) - 1,
                cy = (mydisp.height() / 2) - 1;

  mydisp.clearScreen();
  n = min(mydisp.width(), mydisp.height());
  for (i = n; i > 0; i -= 6) {
    i2    = i / 2;
    start = micros();
    mydisp.fillRect(cx - i2, cy - i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    mydisp.drawRect(cx - i2, cy - i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = mydisp.width(), h = mydisp.height(), r2 = radius * 2;

  mydisp.clearScreen();
  start = micros();
  for (x = radius; x < w; x += r2) {
    for (y = radius; y < h; y += r2) {
      mydisp.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                      w = mydisp.width()  + radius,
                      h = mydisp.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for (x = 0; x < w; x += r2) {
    for (y = 0; y < h; y += r2) {
      mydisp.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = mydisp.width()  / 2 - 1,
                      cy = (mydisp.height() / 2) - 1;

  mydisp.clearScreen();
  n     = min(cx, cy);
  start = micros();
  for (i = 0; i < n; i += 5) {
    mydisp.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      mydisp.Color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = (mydisp.width() / 2) - 1,
                   cy = mydisp.height() / 2 - 1;

  mydisp.clearScreen();
  start = micros();
  for (i = min(cx, cy); i > 10; i -= 5) {
    start = micros();
    mydisp.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     mydisp.Color565(0, i, i));
    t += micros() - start;
    mydisp.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     mydisp.Color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = (mydisp.width()  / 2) - 1,
                cy = (mydisp.height() / 2) - 1;

  mydisp.clearScreen();
  w     = min(mydisp.width(), mydisp.height());
  start = micros();
  for (i = 0; i < w; i += 6) {
    i2 = i / 2;
    mydisp.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, mydisp.Color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = (mydisp.width()  / 2) - 1,
                cy = (mydisp.height() / 2) - 1;

  mydisp.clearScreen();
  start = micros();
  for (i = min(mydisp.width(), mydisp.height()); i > 20; i -= 6) {
    i2 = i / 2;
    mydisp.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, mydisp.Color565(0, i, 0));
  }

  return micros() - start;
}

void GfxBenchmark(){
  Serial.println("\nBenchmark                Time (microseconds)");
  Serial.print("Screen fill              ");
  Serial.println(testFillScreen());
  delay(500);

  Serial.print("Text                     ");
  Serial.println(testText());
  delay(3000);

  Serial.print("Text2                    ");
  Serial.println(testText2());
  delay(3000);

  Serial.print("Lines                    ");
  Serial.println(testLines(CYAN));
  delay(500);

  Serial.print("Horiz/Vert Lines         ");
  Serial.println(testFastLines(RED, BLUE));
  delay(500);
  Serial.print("Rectangles (outline)     ");
  Serial.println(testRects(GREEN));
  delay(500);

  Serial.print("Rectangles (filled)      ");
  Serial.println(testFilledRects(YELLOW, MAGENTA));
  delay(500);

  Serial.print("Circles (filled)         ");
  Serial.println(testFilledCircles(5, MAGENTA));

  Serial.print("Circles (outline)        ");
  Serial.println(testCircles(5, WHITE));
  delay(500);

  Serial.print("Triangles (outline)      ");
  Serial.println(testTriangles());
  delay(500);

  Serial.print("Triangles (filled)       ");
  Serial.println(testFilledTriangles());
  delay(500);

  Serial.print("Rounded rects (outline)  ");
  Serial.println(testRoundRects());
  delay(500);

  Serial.print("Rounded rects (filled)   ");
  Serial.println(testFilledRoundRects());
  delay(500);

  Serial.println("Done!");

  for (uint8_t rotation = 0; rotation < 4; rotation++) {
    mydisp.setRotation(rotation);
    testText();
    delay(2000);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct pt3d
{
  int16_t x, y, z;
};

struct surface
{
  uint8_t p[4];
  int16_t z;
};

struct pt2d
{
  int16_t x, y;
  unsigned is_visible;
};


// define a value that corresponds to "1"
#define U 50

// eye to screen distance (fixed)
#define ZS U

// cube edge length is 2*U
struct pt3d cube[8] =
{
  { -U, -U, U},
  { U, -U, U},
  { U, -U, -U},
  { -U, -U, -U},
  { -U, U, U},
  { U, U, U},
  { U, U, -U},
  { -U, U, -U},
};

// define the surfaces
struct surface cube_surface[6] =
{
  { {0, 1, 2, 3}, 0 },  // bottom
  { {4, 5, 6, 7}, 0 },  // top
  { {0, 1, 5, 4}, 0 },  // back

  { {3, 7, 6, 2}, 0 },  // front
  { {1, 2, 6, 5}, 0 },  // right
  { {0, 3, 7, 4}, 0 },  // left
};

// define some structures for the copy of the box, calculation will be done there
struct pt3d cube2[8];
struct pt2d cube_pt[8];

// will contain a rectangle border of the box projection into 2d plane
int16_t x_min, x_max;
int16_t y_min, y_max;

const int16_t sin_tbl[65] = {
  0, 1606, 3196, 4756, 6270, 7723, 9102, 10394, 11585, 12665, 13623, 14449, 15137, 15679, 16069, 16305, 16384, 16305, 16069, 15679,
  15137, 14449, 13623, 12665, 11585, 10394, 9102, 7723, 6270, 4756, 3196, 1606, 0, -1605, -3195, -4755, -6269, -7722, -9101, -10393,
  -11584, -12664, -13622, -14448, -15136, -15678, -16068, -16304, -16383, -16304, -16068, -15678, -15136, -14448, -13622, -12664, -11584, -10393, -9101, -7722,
  -6269, -4755, -3195, -1605, 0
};

const int16_t cos_tbl[65] = {
  16384, 16305, 16069, 15679, 15137, 14449, 13623, 12665, 11585, 10394, 9102, 7723, 6270, 4756, 3196, 1606, 0, -1605, -3195, -4755,
  -6269, -7722, -9101, -10393, -11584, -12664, -13622, -14448, -15136, -15678, -16068, -16304, -16383, -16304, -16068, -15678, -15136, -14448, -13622, -12664,
  -11584, -10393, -9101, -7722, -6269, -4755, -3195, -1605, 0, 1606, 3196, 4756, 6270, 7723, 9102, 10394, 11585, 12665, 13623, 14449,
  15137, 15679, 16069, 16305, 16384
};


void copy_cube(void)
{
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    cube2[i] = cube[i];
  }
}

void rotate_cube_y(uint16_t w)
{
  uint8_t i;
  int16_t x, z;
  /*
    x' = x * cos(w) + z * sin(w)
    z' = - x * sin(w) + z * cos(w)
  */
  for (i = 0; i < 8; i++)
  {
    x = ((int32_t)cube2[i].x * (int32_t)cos_tbl[w] + (int32_t)cube2[i].z * (int32_t)sin_tbl[w]) >> 14;
    z = (-(int32_t)cube2[i].x * (int32_t)sin_tbl[w] + (int32_t)cube2[i].z * (int32_t)cos_tbl[w]) >> 14;
    //printf("%d: %d %d --> %d %d\n", i, cube2[i].x, cube2[i].z, x, z);
    cube2[i].x = x;
    cube2[i].z = z;
  }
}

void rotate_cube_x(uint16_t w)
{
  uint8_t i;
  int16_t y, z;
  for (i = 0; i < 8; i++)
  {
    y = ((int32_t)cube2[i].y * (int32_t)cos_tbl[w] + (int32_t)cube2[i].z * (int32_t)sin_tbl[w]) >> 14;
    z = (-(int32_t)cube2[i].y * (int32_t)sin_tbl[w] + (int32_t)cube2[i].z * (int32_t)cos_tbl[w]) >> 14;
    cube2[i].y = y;
    cube2[i].z = z;
  }
}

void trans_cube(uint16_t z)
{
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    cube2[i].z += z;
  }
}

void reset_min_max(void)
{
  x_min = 0x07fff;
  y_min = 0x07fff;
  x_max = -0x07fff;
  y_max = -0x07fff;
}

// calculate xs and ys from a 3d value
void convert_3d_to_2d(struct pt3d *p3, struct pt2d *p2)
{
  int32_t t;
  p2->is_visible = 1;
  if (p3->z >= ZS)
  {
    t = ZS;
    t *= p3->x;
    t <<= 1;
    t /= p3->z;
    if (t >= -(mydisp.width()/2) && t <= (mydisp.width()/2) - 1)
    {
      t += (mydisp.width()/2);
      p2->x = t;

      if (x_min > t) x_min = t;
      if (x_max < t) x_max = t;

      t = ZS;
      t *= p3->y;
      t <<= 1;
      t /= p3->z;
      if (t >= -(mydisp.height()/2) && t <= (mydisp.height()/2) - 1)
      {
        t += (mydisp.height()/2);
        p2->y = t;
        if (y_min > t) y_min = t;
        if (y_max < t) y_max = t;
      }
      else
      {
        p2->is_visible = 0;
      }
    }
    else
    {
      p2->is_visible = 0;
    }
  }
  else
  {
    p2->is_visible = 0;
  }
}

void convert_cube(void)
{
  uint8_t i;
  reset_min_max();
  for (i = 0; i < 8; i++)
  {
    convert_3d_to_2d(cube2 + i, cube_pt + i);
  }
}

void calculate_z(void)
{
  uint8_t i, j;
  uint16_t z;
  for (i = 0; i < 6; i++)
  {
    z = 0;
    for (j = 0; j < 4; j++)
    {
      z += cube2[cube_surface[i].p[j]].z;
    }
    z /= 4;
    cube_surface[i].z = z;
    //printf("%d: z=%d\n", i, z);
  }
}

void draw_cube(void)
{
  uint8_t i, ii;
  uint8_t skip_cnt = 3;   /* it is known, that the first 3 surfaces are invisible */
  int16_t z, z_upper;
  uint16_t color;

  z_upper = 32767;
  for (;;)
  {
    ii = 6;
    z = -32767;
    for (i = 0; i < 6; i++)
    {
      if (cube_surface[i].z <= z_upper)
      {
        if (z < cube_surface[i].z)
        {
          z = cube_surface[i].z;
          ii = i;
        }
      }
    }

    if (ii >= 6) break;
    z_upper = cube_surface[ii].z;
    cube_surface[ii].z++;

    if (skip_cnt > 0)
    {
      skip_cnt--;
    }
    else
    {
      color = mydisp.Color565((uint8_t)(((ii + 1) & 1) * 255), (uint8_t)((((ii + 1) >> 1) & 1) * 255), (uint8_t)((((ii + 1) >> 2) & 1) * 255));
      #if defined(_WIREFRAME)
      mydisp.drawQuad(
        cube_pt[cube_surface[ii].p[0]].x, cube_pt[cube_surface[ii].p[0]].y,
        cube_pt[cube_surface[ii].p[1]].x, cube_pt[cube_surface[ii].p[1]].y,
        cube_pt[cube_surface[ii].p[2]].x, cube_pt[cube_surface[ii].p[2]].y,
        cube_pt[cube_surface[ii].p[3]].x, cube_pt[cube_surface[ii].p[3]].y, color);
      #else
      mydisp.fillQuad(
        cube_pt[cube_surface[ii].p[0]].x, cube_pt[cube_surface[ii].p[0]].y,
        cube_pt[cube_surface[ii].p[1]].x, cube_pt[cube_surface[ii].p[1]].y,
        cube_pt[cube_surface[ii].p[2]].x, cube_pt[cube_surface[ii].p[2]].y,
        cube_pt[cube_surface[ii].p[3]].x, cube_pt[cube_surface[ii].p[3]].y, color);
       #endif
    }
  }
}

void calc_and_draw(int16_t w, int16_t v)
{

  copy_cube();
  rotate_cube_y(w);
  rotate_cube_x(v);
  trans_cube(U * 8);
  convert_cube();
  calculate_z();
  draw_cube();
}

int16_t w = 0;
int16_t v = 0;

void cubeDemo(void)
{
  appmode=1;
  btnid = 0;
  mydisp.clearScreen();
  while (1){
    if (btnid == 4) {break;}
    
    calc_and_draw(w, v >> 3);
    v += 3;
    v &= 511;
    w++;
    w &= 63;
    delay(10);
    mydisp.fillRect(x_min, y_min, x_max - x_min + 3, y_max - y_min + 3, 0x0000);
  }
  appmode=0;
  btncounter++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



