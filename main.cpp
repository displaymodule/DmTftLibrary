#include "mbed.h"

#include "DmTftHX8353C.h"
#include "DmTftS6D0164.h"
#include "DmTftIli9325.h"
#include "DmTftIli9341.h"
#include "DmTouch.h"

//DmTftHX8353C tft;  /* DM_TFT18_101 */
//DmTftS6D0164 tft;  /* DM_TFT22_102 */
//DmTftIli9325 tft;  /* DM_TFT28_103 and DM_TFT24_104 */
DmTftIli9341 tft;  /* DM_TFT28_105 */

DmTouch touch(DmTouch::DM_TFT28_105);

int bmpWidth, bmpHeight;
uint8_t bmpImageoffset;
extern uint8_t dmlogo[];

//#define log(...) printf(__VA_ARGS__)
#define log(...)

// LITTLE ENDIAN!
uint16_t read16(uint8_t *src)
{
  uint16_t d;
  uint8_t b;
  b = *src;
  d = *(src+1);
  d <<= 8;
  d |= b;
  return d;
}

// LITTLE ENDIAN!
uint32_t read32(uint8_t *src)
{
  uint32_t d;
  uint16_t b;

  b = read16(src);
  d = read16(src+2);
  d <<= 16;
  d |= b;
  return d;
}

void drawBmpFromFlash(int x, int y)
{
  uint16_t pos = bmpImageoffset;

  uint16_t p;  // pixel
  uint8_t g, b;
  int i, j; // line, column

  for(i=bmpHeight; i>0; i--) {
    for(j=0; j<bmpWidth; j++) {
      b = *(dmlogo+pos++);
      g = *(dmlogo+pos++);
      p = *(dmlogo+pos++);

      p >>= 3;
      p <<= 6;

      g >>= 2;
      p |= g;
      p <<= 5;

      b >>= 3;
      p |= b;

      // write out the 16 bits of color
      tft.setPixel(j, i+y, p);
    }
  }
}


int bmpReadHeader() {
  uint32_t fileSize;
  uint32_t headerSize;
  uint16_t bmpDepth;
  uint16_t pos = 0;
  log("reading bmp header\r\n");
  log("Magic byte is: %d \r\n", read16(dmlogo));

  if (read16(dmlogo) !=0x4D42){ // read magic byte
    log("Magic byte not found\r\n");
    return false;
  }
  pos += 2;

  // read file size
  fileSize = read32(dmlogo+pos);
  log("filesize is: %d \r\n", fileSize);
  log("");
  pos += 4;

  pos += 4; // Skip creator bytes

  bmpImageoffset = read32(dmlogo+pos);
  pos += 4;

  // read DIB header
  headerSize = read32(dmlogo+pos);
  pos +=4;
  bmpWidth = read32(dmlogo+pos);
  pos += 4;
  bmpHeight = read32(dmlogo+pos);
  pos += 4;

  log("Image size:        %d\r\n", fileSize);
  log("Image offset:      %d\r\n", bmpImageoffset);
  log("Header size:       %d\r\n", headerSize);
  log("Image width:       %d\r\n", bmpWidth );
  log("Image height:      %d\r\n", bmpHeight );

  if (read16(dmlogo+pos) != 1){
    // number of color planes must be 1
    return false;
  }
  pos += 2;

  bmpDepth = read16(dmlogo+pos);
  pos +=2;
  log("Bitdepth:          %d\r\n", bmpDepth);

  if (read16(dmlogo+pos) != 0) {
    // compression not supported!
    return false;
  }
  pos += 2; // Should really be 2??

  return true;
}

int main() {
  log("init tft \r\n");
  tft.init();

#if 0
  uint16_t colors[] = { RED, YELLOW, GREEN, CYAN };
  int c = 0;
  int r;
  int x = tft.width()/2;
  int y = (3*tft.height())/4;
  while (1) {
    for (r = 1; r < 100; r+=5) {
      tft.drawCircle(x, y, r, colors[c]);
    }
    for (; r >= 1; r-=5) {
      tft.drawCircle(x, y, r, BLACK);
    }
    x += 20;
    y -= 20;
    c = (c + 1) % 4;
  }
#elif 0
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t w = tft.width();
  uint16_t h = tft.height();
  bool down = false;
  bool lastDown = false;
  tft.drawString(20, 20, "x:");
  tft.drawString(100, 20, "y:");
  touch.begin();
  while (1) {
    touch.readTouchData(x, y, down);
    if (down) {
      tft.drawNumber(40, 20, x, 5, false);
      tft.drawNumber(120, 20, y, 5, false);
    } else if (lastDown) {
      // no longer pressed, clean text
      tft.drawString(40, 20, "     ");
      tft.drawString(120, 20, "     ");
    }
    wait(0.040);
    lastDown = down;
  }
#elif 0
  uint16_t margin = 10;
  int hnum = 3;
  int vnum = 4;
  uint16_t width = (tft.width()-margin)/hnum - margin;
  uint16_t height = (tft.height()-margin)/vnum - margin;
  int numBytes = sizeof(uint16_t)*width*height;
  uint16_t* data = (uint16_t*)malloc(numBytes);

  tft.clearScreen();

  uint16_t part = 0xff/(hnum*vnum);
  for (int i = 0; i < hnum*vnum; i++) {
    memset(data, 0xff - part*i, numBytes);
    tft.drawImage(margin + (width + margin)*(i%hnum),
                  margin + (height + margin)*(i/hnum),
                  width, height, data);
  }
  while(1) {
  }

#else
  tft.drawString(0,32,"www.");
  tft.drawString(12,48,"displaymodule");
  tft.drawString(88,64,".com");

  tft.drawRectangle(20,85,40,105,GREEN);
  tft.drawCircle(60,95,10,BLUE);
  tft.drawTriangle(90,85, 80,105, 100,105, RED);

  if (! bmpReadHeader()) {
    //log("bad bmp\r\n");
    return -1;
  }

  drawBmpFromFlash(0, 0);
  drawBmpFromFlash(0, 130);

//  wait(2);
//  tft.drawNumber(25, 160, 123, 1);
//  tft.drawNumber(25, 180, 123, 2);
//  tft.drawNumber(25, 200, 123, 5);

//  tft.drawNumber(100, 160, 456, 1);
//  tft.drawNumber(100, 180, 456, 2);
//  tft.drawNumber(100, 200, 546, 5);

//  tft.drawChar(150, 130, 'x', 1);
//  tft.drawChar(150, 130, 'y', 1);
//  tft.drawChar(150, 130, 'z', 1);

//  tft.drawChar(150, 150, 'a', 0);
//  tft.drawChar(150, 150, 'b', 0);
//  tft.drawChar(150, 150, 'c', 0);

//  tft.fillRectangle(18, 108, 122, 132, YELLOW);
//  tft.drawNumber(20, 110, 0, 5, false);
//  wait(3);
//  for (int i = -1050; i < 1050; i++) {
//    wait(0.01);
//    tft.drawNumber(20, 110, i, 5, false);
//    tft.drawNumber(65, 110, -i, 5, true);
//    tft.drawNumber(20, 128, -i, 5, true);
//    tft.setTextColor(BLACK, GREEN);
//    tft.drawNumber(65, 128, i, 5, false);
//    tft.setTextColor(BLACK, WHITE);
//  }
  while(1) {
  }
#endif
}
