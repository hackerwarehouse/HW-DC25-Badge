#include <SPI.h>
#include <SSD_13XX.h>

#include "core.h"

extern SSD_13XX mydisp;

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
  mydisp.println("I implore thee,");
  mydisp.setTextScale(1);
  mydisp.println("my foonting turlingdromes.");
  mydisp.println("And hooptiously drangle me");
  mydisp.println("with crinkly bindlewurdles,");
  mydisp.println("Or I will rend thee");
  mydisp.println("in the gobberwarts");
  mydisp.println("with my blurglecruncheon,");
  mydisp.println("see if I don't!");
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

void Benchmark(){
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
