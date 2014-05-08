/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained. 

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/
#include <SPI.h>
#include <DmTftIli9325.h>

int bmpWidth, bmpHeight;
uint8_t bmpImageoffset;
extern uint8_t dmlogo[];

DmTftIli9325 tft = DmTftIli9325(18, 17, 19, 16);

void setup()
{
  tft.init();
  
  if (! bmpReadHeader()) { 
    return;
  }

  drawBmpFromFlash(0, 0);
  drawBmpFromFlash(0, 130);
   
  tft.drawString(0,32,"www.");
  tft.drawString(12,48,"displaymodule");
  tft.drawString(88,64,".com");

  tft.drawRectangle(20,85,40,105,GREEN); 
  tft.drawCircle(60,95,10,BLUE);
  tft.drawTriangle(90,85, 80,105,100,105, RED);
}

void loop()
{
}

// Info about bmp format can be found here: http://en.wikipedia.org/wiki/BMP_file_format
void drawBmpFromFlash(int x, int y) 
{
  uint16_t pos = bmpImageoffset;

  uint32_t time = millis();
  uint16_t p;  // pixel 
  uint8_t g, b;
  int i, j; // line, column

  for(i=bmpHeight; i>0; i--) {
    for(j=0; j<bmpWidth; j++) { 
      // convert pixel from 888 to 565
      b = pgm_read_byte_near(dmlogo+pos++);     // blue
      g = pgm_read_byte_near(dmlogo+pos++);     // green
      p = pgm_read_byte_near(dmlogo+pos++);      // red

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


boolean bmpReadHeader() {
  uint32_t fileSize;
  uint32_t headerSize;
  uint16_t bmpDepth;
  uint16_t pos = 0;
  
  if (pgm_read_word_near(dmlogo) !=0x4D42){ // read magic byte
    return false;
  }
  pos += 2;

  // read file size
  fileSize = pgm_read_dword_near(dmlogo+pos);
  pos += 4;

  pos += 4; // Skip creator bytes
  
  bmpImageoffset = pgm_read_dword_near(dmlogo+pos);
  pos += 4;

  // read DIB header
  headerSize = pgm_read_dword_near(dmlogo+pos);
  pos +=4;
  bmpWidth = pgm_read_dword_near(dmlogo+pos);
  pos += 4;
  bmpHeight = pgm_read_dword_near(dmlogo+pos);
  pos += 4;

  if (pgm_read_word_near(dmlogo+pos) != 1){
    // number of color planes must be 1
    return false;
  }
  pos += 2;
  
  bmpDepth = pgm_read_word_near(dmlogo+pos);
  pos +=2;

  if (pgm_read_dword_near(dmlogo+pos) != 0) {
    // compression not supported!
    return false;
  }
  pos += 2;

  return true;
}